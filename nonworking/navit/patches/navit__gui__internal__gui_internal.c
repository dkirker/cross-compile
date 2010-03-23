diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index 4dadb3f..488b387 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -5984,7 +5984,7 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
 	      this->text_foreground_color=*attr->u.color;
 	else
 	      this->text_foreground_color=COLOR_WHITE;
-	this->text_background_color=COLOR_BLACK;
+	this->text_background_color=COLOR_WHITE;
 	if( (attr=attr_search(attrs,NULL,attr_columns)))
 	      this->cols=attr->u.num;
 	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
