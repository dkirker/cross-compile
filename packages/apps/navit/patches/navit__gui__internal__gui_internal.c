diff --git a/navit/navit/gui/internal/gui_internal.c b/navit/navit/gui/internal/gui_internal.c
index 97f36f4..d0f8d83 100644
--- a/navit/navit/gui/internal/gui_internal.c
+++ b/navit/navit/gui/internal/gui_internal.c
@@ -1607,6 +1607,7 @@ gui_internal_top_bar(struct gui_priv *this)
 		if (g_list_previous(l) || !g_list_next(l)) {
 			wc=l->data;
 			wcn=gui_internal_label_new(this, wc->text);
+//			wcn->flags |= flags_fill | gravity_ycenter;
 			wcn->foreground=foreground;
 			if (g_list_next(l))
 				use_sep=1;
@@ -1620,6 +1621,7 @@ gui_internal_top_bar(struct gui_priv *this)
 			}
 			if (use_sep) {
 				struct widget *wct=gui_internal_label_new(this, "»");
+//				wct->flags |= flags_fill | gravity_ycenter;
 				wct->foreground=foreground;
 				res=g_list_prepend(res, wct);
 				width_used+=sep_len+w->spx;
@@ -1648,6 +1650,7 @@ gui_internal_top_bar(struct gui_priv *this)
 			wc=l->data;
 		}
 		wcn=gui_internal_label_new(this, ".. »");
+//		wcn->flags |= flags_fill | gravity_ycenter;
 		wcn->foreground=foreground;
 		wcn->func=gui_internal_cmd_return;
 		wcn->data=wc;
