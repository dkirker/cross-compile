diff --git a/navit/navit/navit.c b/navit/navit/navit.c
index 320ba32..6091263 100644
--- a/navit/navit/navit.c
+++ b/navit/navit/navit.c
@@ -2232,7 +2232,7 @@ navit_layout_switch(struct navit *n)
 	}
 	
 	//We calculate sunrise anyway, cause it is needed both for day and for night
-        if (__sunriset__(year,month,day,geo_attr.u.coord_geo->lat,geo_attr.u.coord_geo->lng,35,1,&trise,&tset)!=0) {
+        if (__sunriset__(year,month,day,geo_attr.u.coord_geo->lng,geo_attr.u.coord_geo->lat,-5,1,&trise,&tset)!=0) {
 		//near the pole sun never rises/sets, so we should never switch profiles
 		n->prevTs=currTs;
 		return;
@@ -2251,7 +2251,7 @@ navit_layout_switch(struct navit *n)
 	    }
 	}
 	if (l->nightname) {
-	    if (__sunriset__(year,month,day,geo_attr.u.coord_geo->lat,geo_attr.u.coord_geo->lng,-24,0,&trise,&tset)!=0) {
+	    if (__sunriset__(year,month,day,geo_attr.u.coord_geo->lng,geo_attr.u.coord_geo->lat,-5,1,&trise,&tset)!=0) {
 		//near the pole sun never rises/sets, so we should never switch profiles
 		n->prevTs=currTs;
 		return;
