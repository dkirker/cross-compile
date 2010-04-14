diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index 4dadb3f..914812a 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -2642,6 +2642,8 @@ static void gui_internal_keypress_do(struct gui_priv *this, char *key)
 	int len=0;
 	char *text=NULL;
 
+	dbg(1,"got key %s\n",key);
+
 	menu=g_list_last(this->root.children)->data;
 	wi=gui_internal_find_widget(menu, NULL, STATE_EDIT);
 	if (wi) {
@@ -2892,8 +2894,8 @@ static void
 gui_internal_search_idle_start(struct gui_priv *this, char *wm_name, struct widget *search_list, void *param)
 {
 	this->idle_cb=callback_new_4(callback_cast(gui_internal_search_idle), this, wm_name, search_list, param);
-	this->idle=event_add_idle(50,this->idle_cb);
 	callback_call_0(this->idle_cb);
+	this->idle=event_add_idle(50,this->idle_cb);
 }
 
 
@@ -2949,10 +2951,10 @@ gui_internal_keyboard_key_data(struct gui_priv *this, struct widget *wkbd, char
 		NULL, gravity_center|orientation_vertical, func, data));
 	wk->data_free=data_free;
 	wk->background=this->background;
-	wk->bl=w/2;
-	wk->br=0;
-	wk->bt=h/3;
-	wk->bb=0;
+	wk->bl=(w > wk->w) ? ((w - wk->w) / 8 ): 0; //w/2;
+	wk->br=(w > wk->w) ? ((w - wk->w) / 8 ): 0; //0;
+	wk->bt=(h > wk->h) ? ((h - wk->h) / 8 ): 0; //0; //h/3;
+	wk->bb=(h > wk->h) ? ((h - wk->h) / 8 ): 0; //0;
 	return wk;
 }
 
@@ -5984,7 +5986,7 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
 	      this->text_foreground_color=*attr->u.color;
 	else
 	      this->text_foreground_color=COLOR_WHITE;
-	this->text_background_color=COLOR_BLACK;
+	this->text_background_color=COLOR_WHITE;
 	if( (attr=attr_search(attrs,NULL,attr_columns)))
 	      this->cols=attr->u.num;
 	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
