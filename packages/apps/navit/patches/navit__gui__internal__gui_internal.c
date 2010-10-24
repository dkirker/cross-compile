diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index 78d6886..61a42f6 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -6487,7 +6487,9 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
 	      this->text_foreground_color=*attr->u.color;
 	else
 	      this->text_foreground_color=COLOR_WHITE;
+#ifndef USE_WEBOS
 	this->text_background_color=COLOR_BLACK;
+#endif
 	if( (attr=attr_search(attrs,NULL,attr_columns)))
 	      this->cols=attr->u.num;
 	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
