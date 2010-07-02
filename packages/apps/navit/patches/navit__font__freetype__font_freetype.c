diff --git a/navit/navit/font/freetype/font_freetype.c b/navit/navit/font/freetype/font_freetype.c
index 74c8f25..7e394ea 100644
--- a/navit/navit/font/freetype/font_freetype.c
+++ b/navit/navit/font/freetype/font_freetype.c
@@ -275,6 +275,9 @@ font_freetype_text_new(char *text, struct font_freetype_font *font, int dx,
  */
 static char *fontfamilies[] = {
 	"Liberation Sans",
+#ifdef USE_WEBOS
+	"Prelude",
+#endif
 	"Arial",
 	"NcrBI4nh",
 	"luximbi",
