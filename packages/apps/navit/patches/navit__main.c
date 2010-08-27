diff --git a/navit/navit/main.c b/navit/navit/main.c
index f4f5356..72e149e 100644
--- a/navit/navit/main.c
+++ b/navit/navit/main.c
@@ -168,7 +168,7 @@ main_init(const char *program)
 	int l;
 
 #ifndef _WIN32
-	signal(SIGCHLD, sigchld);
+//	signal(SIGCHLD, sigchld);
 #endif
 	cbl=callback_list_new();
 #ifdef HAVE_API_WIN32_BASE
