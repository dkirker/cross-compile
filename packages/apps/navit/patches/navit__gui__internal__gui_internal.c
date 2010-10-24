diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index 78d6886..cbebfbe 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -1575,6 +1575,7 @@ gui_internal_top_bar(struct gui_priv *this)
 		if (g_list_previous(l) || !g_list_next(l)) {
 			wc=l->data;
 			wcn=gui_internal_label_new(this, wc->text);
+			wcn->flags |= flags_fill | gravity_ycenter;
 			wcn->foreground=foreground;
 			if (g_list_next(l))
 				use_sep=1;
@@ -1588,6 +1589,7 @@ gui_internal_top_bar(struct gui_priv *this)
 			}
 			if (use_sep) {
 				struct widget *wct=gui_internal_label_new(this, "»");
+				wct->flags |= flags_fill | gravity_ycenter;
 				wct->foreground=foreground;
 				res=g_list_prepend(res, wct);
 				width_used+=sep_len+w->spx;
@@ -1616,6 +1618,7 @@ gui_internal_top_bar(struct gui_priv *this)
 			wc=l->data;
 		}
 		wcn=gui_internal_label_new(this, ".. »");
+		wcn->flags |= flags_fill | gravity_ycenter;
 		wcn->foreground=foreground;
 		wcn->func=gui_internal_cmd_return;
 		wcn->data=wc;
@@ -6487,7 +6490,9 @@ static struct gui_priv * gui_internal_new(struct navit *nav, struct gui_methods
 	      this->text_foreground_color=*attr->u.color;
 	else
 	      this->text_foreground_color=COLOR_WHITE;
+#ifndef USE_WEBOS
 	this->text_background_color=COLOR_BLACK;
+#endif
 	if( (attr=attr_search(attrs,NULL,attr_columns)))
 	      this->cols=attr->u.num;
 	if( (attr=attr_search(attrs,NULL,attr_osd_configuration)))
