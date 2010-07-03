diff --git a/navit/navit/speech/speech_dispatcher/speech_speech_dispatcher.c b/navit/navit/speech/speech_dispatcher/speech_speech_dispatcher.c
index b2c0e2a..3db7d8a 100644
--- a/navit/navit/speech/speech_dispatcher/speech_speech_dispatcher.c
+++ b/navit/navit/speech/speech_dispatcher/speech_speech_dispatcher.c
@@ -27,6 +27,7 @@
 #include <glib.h>
 #include <stdarg.h>
 #include "config.h"
+#include "item.h"
 #include <libspeechd.h>
 #include "plugin.h"
 #include "speech.h"
@@ -59,8 +60,11 @@ static struct speech_methods speechd_meth = {
 static struct speech_priv *
 speechd_new(struct speech_methods *meth, struct attr **attrs) {
 	struct speech_priv *this;
+	struct attr *volume, *language;
+	char *lang_str;
 	SPDConnection *conn;
 
+
 	conn = spd_open("navit","main",NULL,SPD_MODE_SINGLE);
 	if (! conn) 
 		return NULL;
@@ -68,7 +72,33 @@ speechd_new(struct speech_methods *meth, struct attr **attrs) {
 	if (this) {
 		this->conn=conn;
 		*meth=speechd_meth;
+		volume = attr_search(attrs, NULL, attr_data);
+		language = attr_search(attrs, NULL, attr_language);
+		if (language) {
+			lang_str=g_strdup(language->u.str);
+		} else {
+			char *lang_env=getenv("LANG");
+
+			if (lang_env) {
+				char *country,*lang,*lang_full;
+				lang_full=g_strdup(lang_env);
+				strtolower(lang_full,lang_env);
+				lang=g_strdup(lang_full);
+				country=strchr(lang_full,'_');
+				if (country) {
+					lang[country-lang_full]='\0';
+					*country++='-';
+				}
+				lang_str=g_strdup(lang);
+				g_free(lang_full);
+				g_free(lang);
+			}
+		}
 		spd_set_punctuation(conn, SPD_PUNCT_NONE);
+		spd_set_volume(conn, volume->u.num);
+		spd_set_language(conn, lang_str);
+		if (lang_str)
+			g_free(lang_str);
 	}
 	return this;
 }
