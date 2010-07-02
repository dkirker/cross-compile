diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index f0b051e..b2daf0a 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -3171,10 +3171,17 @@ gui_internal_keyboard_key_data(struct gui_priv *this, struct widget *wkbd, char
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
 
@@ -6250,7 +6257,11 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
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
