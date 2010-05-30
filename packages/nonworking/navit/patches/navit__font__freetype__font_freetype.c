diff --git a/navit/navit/font/freetype/font_freetype.c b/navit/navit/font/freetype/font_freetype.c
index 6add9ad..8dc5815 100644
--- a/navit/navit/font/freetype/font_freetype.c
+++ b/navit/navit/font/freetype/font_freetype.c
@@ -254,6 +254,9 @@ font_freetype_text_new(char *text, struct font_freetype_font *font, int dx,
  */
 static char *fontfamilies[] = {
 	"Liberation Sans",
+#ifdef USE_WEBOS
+	"Prelude",
+#endif
 	"Arial",
 	"NcrBI4nh",
 	"luximbi",
