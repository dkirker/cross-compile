diff --git a/navit/navit/graphics.c b/navit/navit/graphics.c
index 6103f7d..627946e 100644
--- a/navit/navit/graphics.c
+++ b/navit/navit/graphics.c
@@ -1237,7 +1237,7 @@ graphics_draw_polyline_as_polygon(struct graphics *gra, struct graphics_gc *gc,
 			l=1;
 		if (wi*lscale > 10000)
 			lscale=10000/wi;
-		dbg_assert(wi*lscale < 10000);
+		dbg_assert(wi*lscale <= 10000);
 		calc_offsets(wi*lscale, l, dx, dy, &o);
 		pos.x = pnt[i].x + o.ny;
 		pos.y = pnt[i].y + o.px;
