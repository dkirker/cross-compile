diff --git a/navit/navit/route.c b/navit/navit/route.c
index 1411522..2efe55c 100644
--- a/navit/navit/route.c
+++ b/navit/navit/route.c
@@ -664,7 +664,7 @@ route_path_update_done(struct route *this, int new_graph)
 			/* FIXME */
 			int seg_time=route_time_seg(this->vehicleprofile, seg->data, NULL);
 			if (seg_time == INT_MAX) {
-				dbg(0,"error\n");
+				dbg(1,"error\n");
 			} else
 				path_time+=seg_time;
 			path_len+=seg->data->len;
