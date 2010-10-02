diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index d2bd9e1..03a3247 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -71,6 +71,7 @@
 #include "util.h"
 #include "bookmarks.h"
 #include "debug.h"
+#include "fib.h"
 
 
 extern char *version;
@@ -192,6 +193,7 @@ struct gui_config_settings {
    */
   int spacing;
 
+  int nb_lines;
 };
 
 /**
@@ -209,9 +211,9 @@ const int SMALL_PROFILE=2;
  * [2] => Small profile (default)
  */
 static struct gui_config_settings config_profiles[]={
-      {545,32,48,96,10}
-    , {300,32,48,64,3}
-      ,{200,16,32,48,2}
+        {545,32,48,96,10,10},
+        {300,32,48,64,3,8},
+        {200,16,32,48,2,5}
 };
 
 struct route_data {
@@ -242,6 +244,7 @@ struct gui_priv {
 	int fullscreen;
 	struct graphics_font *fonts[3];
 	int icon_xs, icon_s, icon_l;
+        int nb_lines;
 	int pressed;
 	struct widget *widgets;
 	int widgets_count;
@@ -1748,6 +1751,14 @@ static void gui_internal_apply_config(struct gui_priv *this)
   {
     this->icon_l = this->config.icon_l;
   }
+  if(this->config.nb_lines == -1 )
+  {
+      this->nb_lines = current_config->nb_lines;
+  }
+  else
+  {
+    this->nb_lines = this->config.nb_lines;
+  }
   if(this->config.spacing == -1 )
   {
     this->spacing = current_config->spacing;
@@ -2198,18 +2209,30 @@ struct selector {
 		type_none}},
 };
 
+union poi_param {
+	guint i;
+	struct {
+		unsigned char sel, selnb, pagenb, dist;
+	} p;
+};
+
 static void gui_internal_cmd_pois(struct gui_priv *this, struct widget *wm, void *data);
 
 static struct widget *
-gui_internal_cmd_pois_selector(struct gui_priv *this, struct pcoord *c)
+gui_internal_cmd_pois_selector(struct gui_priv *this, struct pcoord *c, int pagenb)
 {
 	struct widget *wl,*wb;
 	int i;
 	wl=gui_internal_box_new(this, gravity_left_center|orientation_horizontal|flags_fill);
 	for (i = 0 ; i < sizeof(selectors)/sizeof(struct selector) ; i++) {
-	gui_internal_widget_append(wl, wb=gui_internal_button_new_with_callback(this, NULL,
-		image_new_xs(this, selectors[i].icon), gravity_left_center|orientation_vertical,
-		gui_internal_cmd_pois, &selectors[i]));
+		union poi_param p;
+		p.p.sel = 1;
+		p.p.selnb = i;
+		p.p.pagenb = pagenb;
+		p.p.dist = 0;
+		gui_internal_widget_append(wl, wb=gui_internal_button_new_with_callback(this, NULL,
+			image_new_xs(this, selectors[i].icon), gravity_left_center|orientation_vertical,
+			gui_internal_cmd_pois, GUINT_TO_POINTER(p.i)));
 		wb->c=*c;
 		wb->bt=10;
 	}
@@ -2217,28 +2240,30 @@ gui_internal_cmd_pois_selector(struct gui_priv *this, struct pcoord *c)
 }
 
 static struct widget *
-gui_internal_cmd_pois_item(struct gui_priv *this, struct coord *center, struct item *item, struct coord *c, int dist)
+gui_internal_cmd_pois_item(struct gui_priv *this, struct coord *center, struct item *item, struct coord *c, int dist, char* name)
 {
 	char distbuf[32];
 	char dirbuf[32];
 	char *type;
-	struct attr attr;
 	struct widget *wl,*wt;
 	char *text;
 
 	wl=gui_internal_box_new(this, gravity_left_center|orientation_horizontal|flags_fill);
 
-	sprintf(distbuf,"%d", dist/1000);
+	if (dist > 10000)
+		sprintf(distbuf,"%d", dist/1000);
+	else
+		sprintf(distbuf,"%d.%d", dist/1000, (dist%1000)/100);
 	get_direction(dirbuf, transform_get_angle_delta(center, c, 0), 1);
 	type=item_to_name(item->type);
-	if (item_attr_get(item, attr_label, &attr)) {
-		wl->name=g_strdup_printf("%s %s",type,attr.u.str);
+	if (name[0]) {
+		wl->name=g_strdup_printf("%s %s",type,name);
 	} else {
-		attr.u.str="";
 		wl->name=g_strdup(type);
 	}
-        text=g_strdup_printf("%s %s %s %s", distbuf, dirbuf, type, attr.u.str);
+        text=g_strdup_printf("%s %s %s %s", distbuf, dirbuf, type, name);
 	wt=gui_internal_label_new(this, text);
+
 	wt->datai=dist;
 	gui_internal_widget_append(wl, wt);
 	g_free(text);
@@ -2284,6 +2309,13 @@ gui_internal_cmd_pois_item_selected(struct selector *sel, enum item_type type)
 
 static void gui_internal_cmd_position(struct gui_priv *this, struct widget *wm, void *data);
 
+struct item_data {
+	int dist;
+	char label[32];
+	struct item item;
+	struct coord c;
+};
+
 static void
 gui_internal_cmd_pois(struct gui_priv *this, struct widget *wm, void *data)
 {
@@ -2293,16 +2325,27 @@ gui_internal_cmd_pois(struct gui_priv *this, struct widget *wm, void *data)
 	struct map *m;
 	struct map_rect *mr;
 	struct item *item;
-	int idist,dist=20000;
 	struct widget *wi,*w,*w2,*wb;
 	enum projection pro=wm->c.pro;
-	struct selector *isel=wm->data;
+	union poi_param param = {.i = GPOINTER_TO_UINT(wm->data)};
+	int idist,dist=10000*(param.p.dist+1);
+	struct selector *isel = param.p.sel? &selectors[param.p.selnb]: NULL;
+	int pagenb = param.p.pagenb;
+	int pagesize = this->nb_lines;
+	int maxitem = pagesize*(pagenb+1), it = 0, i;
+	struct item_data *items= malloc(maxitem * sizeof(struct item_data));
+	
+        struct fibheap* fh = fh_makekeyheap();
+	int cnt = 0;
+
+	dbg(2, "Params: sel = %i, selnb = %i, pagenb = %i, dist = %i\n",
+	    param.p.sel, param.p.selnb, param.p.pagenb, param.p.dist);
 
 	wb=gui_internal_menu(this, isel ? isel->name : _("POIs"));
 	w=gui_internal_box_new(this, gravity_top_center|orientation_vertical|flags_expand|flags_fill);
 	gui_internal_widget_append(wb, w);
-	if (! isel)
-		gui_internal_widget_append(w, gui_internal_cmd_pois_selector(this,&wm->c));
+	if (! param.i)
+		gui_internal_widget_append(w, gui_internal_cmd_pois_selector(this,&wm->c,pagenb));
 	w2=gui_internal_box_new(this, gravity_top_center|orientation_vertical|flags_expand|flags_fill);
 	gui_internal_widget_append(w, w2);
 
@@ -2320,14 +2363,24 @@ gui_internal_cmd_pois(struct gui_priv *this, struct widget *wm, void *data)
 				    item_coord_get_pro(item, &c, 1, pro) &&
 				    coord_rect_contains(&sel->u.c_rect, &c) &&
 				    (idist=transform_distance(pro, &center, &c)) < dist) {
-					gui_internal_widget_append(w2, wi=gui_internal_cmd_pois_item(this, &center, item, &c, idist));
-					wi->func=gui_internal_cmd_position;
-					wi->data=(void *)2;
-					wi->item=*item;
-					wi->state |= STATE_SENSITIVE;
-					wi->c.x=c.x;
-					wi->c.y=c.y;
-					wi->c.pro=pro;
+					struct item_data *data;
+					struct attr attr;
+					if (it == maxitem) {
+						data = fh_extractmin(fh);
+					} else {
+						data = &items[it++];
+					}
+					data->item = *item;
+					data->c = c;
+					data->dist = idist;
+					if (item_attr_get(item, attr_label, &attr)) {
+						strncpy(data->label, attr.u.str, sizeof(data->label));
+					} else {
+						data->label[0] = 0;
+					}
+					fh_insertkey(fh, -((idist<<12) + cnt++), data);
+					if (it == maxitem)
+						dist = (-fh_minkey(fh))>>12;
 				}
 			}
 			map_rect_destroy(mr);
@@ -2336,7 +2389,50 @@ gui_internal_cmd_pois(struct gui_priv *this, struct widget *wm, void *data)
 	}
 	map_selection_destroy(sel);
 	mapset_close(h);
-	w2->children=g_list_sort_with_data(w2->children,  gui_internal_cmd_pois_sort_num, (void *)1);
+
+	for (i = it; i > pagenb*pagesize; i--) {
+		int key = fh_minkey(fh);
+		struct item_data *data = fh_extractmin(fh);
+		if (data == NULL)
+		{
+			dbg(2, "Empty heap: maxitem = %i, it = %i, dist = %i\n", maxitem, it, dist);
+			break;
+		}
+		dbg(2, "dist1: %i, dist2: %i\n", data->dist, (-key)>>12);
+		gui_internal_widget_prepend(w2, wi=gui_internal_cmd_pois_item(this, &center, &data->item, &data->c, data->dist, data->label));
+		wi->func=gui_internal_cmd_position;
+		wi->data=(void *)2;
+		wi->item=data->item;
+		wi->state |= STATE_SENSITIVE;
+		wi->c.x=data->c.x;
+		wi->c.y=data->c.y;
+		wi->c.pro=pro;
+	}
+
+	fh_deleteheap(fh);
+	free(items);
+
+	{  // Add an entry for more POI
+		struct widget *wl,*wt;
+		char buffer[32];
+
+		wl=gui_internal_box_new(this, gravity_left_center|orientation_horizontal|flags_fill);
+		if (it == maxitem) {
+			param.p.pagenb++;
+			snprintf(buffer, sizeof(buffer), "Next page (%i)", param.p.pagenb+1);
+		} else {
+			param.p.dist++;
+			snprintf(buffer, sizeof(buffer), "More (%i km)", 10*(param.p.dist+1));
+		}
+		wt=gui_internal_label_new(this, buffer);
+		gui_internal_widget_append(wl, wt);
+		wl->func=gui_internal_cmd_pois;
+		wl->data=GUINT_TO_POINTER(param.i);
+		wl->state |= STATE_SENSITIVE;
+		wl->c = wm->c;
+		gui_internal_widget_append(w2, wl);
+	}
+
 	gui_internal_menu_render(this);
 }
 
@@ -3172,10 +3268,17 @@ gui_internal_keyboard_key_data(struct gui_priv *this, struct widget *wkbd, char
 		NULL, gravity_center|orientation_vertical, func, data));
 	wk->data_free=data_free;
 	wk->background=this->background;
+#ifdef USE_WEBOS
+	wk->bl=(w > wk->w) ? ((w - wk->w) / 8 ): 0;
+	wk->br=(w > wk->w) ? ((w - wk->w) / 8 ): 0;
+	wk->bt=(h > wk->h) ? ((h - wk->h) / 8 ): 0;
+	wk->bb=(h > wk->h) ? ((h - wk->h) / 8 ): 0;
+#else
 	wk->bl=w/2;
 	wk->br=0;
 	wk->bt=h/3;
 	wk->bb=0;
+#endif
 	return wk;
 }
 
@@ -6217,6 +6320,14 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
         {
 	  this->config.icon_s=-1;
 	}
+	if( (attr=attr_search(attrs,NULL,attr_nb_lines)))
+	{
+	  this->config.nb_lines=attr->u.num;
+	}
+	else
+	{
+	  this->config.nb_lines=-1;
+	}
 	if( (attr=attr_search(attrs,NULL,attr_spacing)))
 	{
 	  this->config.spacing=attr->u.num;
@@ -6251,7 +6362,11 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
 	      this->text_foreground_color=*attr->u.color;
 	else
 	      this->text_foreground_color=COLOR_WHITE;
+#ifdef USE_WEBOS
+	this->text_background_color=COLOR_WHITE;
+#else
 	this->text_background_color=COLOR_BLACK;
+#endif
 	if( (attr=attr_search(attrs,NULL,attr_columns)))
 	      this->cols=attr->u.num;
 	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
