diff --git a/navit/navit/graphics/sdl/graphics_sdl.c b/navit/navit/graphics/sdl/graphics_sdl.c
index e8c85de..0786f5a 100644
--- a/navit/navit/graphics/sdl/graphics_sdl.c
+++ b/navit/navit/graphics/sdl/graphics_sdl.c
@@ -33,6 +33,7 @@
 
 #include <SDL/SDL.h>
 #include <math.h>
+#include <PDL.h>
 
 #define RASTER
 #undef SDL_SGE
@@ -43,8 +44,8 @@
 #define SDL_IMAGE
 #undef LINUX_TOUCHSCREEN
 
-#define DISPLAY_W 800
-#define DISPLAY_H 600
+#define DISPLAY_W 320
+#define DISPLAY_H 480
 
 
 #undef DEBUG
@@ -141,6 +142,29 @@ struct graphics_priv {
 
 static int dummy;
 
+// PRE
+struct event_timeout {
+    SDL_TimerID id;
+    int multi;
+    struct callback *cb;
+};
+
+struct idle_task {
+    int priority;
+    struct callback *cb;
+};
+
+struct userevent_data {
+    int is_idle_event;
+};
+
+static int	quit_event_loop=0; // quit the main event loop
+static struct graphics_priv* the_graphics=NULL; 
+static int	the_graphics_count=0; // count how many graphics objects are created
+static struct event_timeout* idle_timer=NULL;
+static int	idle_events_pending=0;
+static GPtrArray *idle_callbacks=NULL;
+// PRE 
 
 struct graphics_font_priv {
 #ifdef SDL_TTF
@@ -167,6 +191,7 @@ struct graphics_image_priv {
     SDL_Surface *img;
 };
 
+static void sdl_idle_timeout_rearm();
 
 #ifdef LINUX_TOUCHSCREEN
 static int input_ts_exit(struct graphics_priv *gr);
@@ -193,7 +218,8 @@ graphics_destroy(struct graphics_priv *gr)
 #ifdef LINUX_TOUCHSCREEN
         input_ts_exit(gr);
 #endif
-        SDL_Quit();
+        PDL_Quit();
+	SDL_Quit();
     }
 
     g_free(gr);
@@ -202,6 +228,7 @@ graphics_destroy(struct graphics_priv *gr)
 /* graphics_font */
 static char *fontfamilies[]={
 	"Liberation Mono",
+	"Prelude",
 	"Arial",
 	"DejaVu Sans",
 	"NcrBI4nh",
@@ -1845,6 +1872,18 @@ static gboolean graphics_sdl_idle(void *data)
     int ret, key;
     char keybuf[2];
 
+    dbg(1,"data=%p\n", data);
+    //Palm Pre mode
+    if(data==NULL) {
+    	if(the_graphics!=NULL) {
+	    gr = the_graphics;
+	} 
+	else {
+	    dbg(0,"graphics_idle: graphics not set!\n");
+    	    return FALSE;
+	}
+    }
+
     /* generate the initial resize callback, so the gui knows W/H
 
        its unsafe to do this directly inside register_resize_callback;
@@ -1947,12 +1986,14 @@ static gboolean graphics_sdl_idle(void *data)
 
     while(1)
     {
-        ret = SDL_PollEvent(&ev);
+        //ret = SDL_PollEvent(&ev);
+        ret = SDL_WaitEvent(&ev);
         if(ret == 0)
         {
             break;
         }
-
+	
+	dbg(1,"SDL_Event %d\n", ev.type);
         switch(ev.type)
         {
             case SDL_MOUSEMOTION:
@@ -2062,6 +2103,7 @@ static gboolean graphics_sdl_idle(void *data)
 
             case SDL_QUIT:
             {
+		quit_event_loop = 1;
                 navit_destroy(gr->nav);
                 break;
             }
@@ -2082,6 +2124,27 @@ static gboolean graphics_sdl_idle(void *data)
                 break;
             }
 
+            case SDL_USEREVENT:
+            {
+		SDL_UserEvent userevent = ev.user;
+                if(userevent.type==SDL_USEREVENT && userevent.code==123 && userevent.data2) {
+    		    struct callback *cb = (struct callback *)userevent.data1;
+		    struct userevent_data * data = (struct userevent_data*)userevent.data2;
+                    dbg(1, "SDL_USEREVENT received cb(%p)\n", cb);
+		    if (data->is_idle_event == 1) {
+			idle_events_pending--;
+			if (!idle_events_pending)
+			    sdl_idle_timeout_rearm();
+		    }
+		    g_free(userevent.data2);
+		    callback_call_0(cb);
+                }
+                else {
+                    dbg(1, "SDL_USEREVENT is not from timer\n");
+                }
+                break;
+            }
+
             default:
             {
 #ifdef DEBUG
@@ -2090,6 +2153,7 @@ static gboolean graphics_sdl_idle(void *data)
                 break;
             }
         }
+	dbg(1, "event processed\n");
     }
 
     return TRUE;
@@ -2099,6 +2163,7 @@ static gboolean graphics_sdl_idle(void *data)
 static struct graphics_priv *
 graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr **attrs, struct callback_list *cbl)
 {
+    dbg(1,"enter\n");
     struct graphics_priv *this=g_new0(struct graphics_priv, 1);
     struct attr *attr;
     int ret;
@@ -2107,30 +2172,38 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     this->nav = nav;
     this->cbl = cbl;
 
-    ret = SDL_Init(SDL_INIT_VIDEO);
+    ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
     if(ret < 0)
     {
+	dbg(0,"SDL_Init failed %d\n", ret);
         g_free(this);
         return NULL;
     }
 
+    PDL_SetOrientation(PDL_ORIENTATION_0);
+    PDL_ScreenTimeoutEnable(PDL_FALSE);
+
 #ifdef SDL_TTF
     ret = TTF_Init();
     if(ret < 0)
     {
-        g_free(this);
-        SDL_Quit();
+        dbg(0,"TTF_Init failed %d\n", ret);
+	g_free(this);
+        PDL_Quit();
+	SDL_Quit();
         return NULL;
     }
 #else
     FT_Init_FreeType( &this->library );
 #endif
 
-    if (! event_request_system("glib","graphics_sdl_new"))
+    if (! event_request_system("sdl","graphics_sdl_new")) {
+	dbg(0,"event_request_system failed");
         return NULL;
+    }
 
     this->video_bpp = 16;
-    this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
+    this->video_flags = SDL_SWSURFACE /*SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE*/;
 
     if ((attr=attr_search(attrs, NULL, attr_w)))
         w=attr->u.num;
@@ -2149,17 +2222,19 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
 
     this->screen = SDL_SetVideoMode(w, h, this->video_bpp, this->video_flags);
 
-    /* Use screen size instead of requested */
-    w = this->screen->w;
-    h = this->screen->h;
-
     if(this->screen == NULL)
     {
+	dbg(0,"SDL_SetVideoMode failed\n");
         g_free(this);
-        SDL_Quit();
+        PDL_Quit();
+	SDL_Quit();
         return NULL;
     }
 
+    /* Use screen size instead of requested */
+    w = this->screen->w;
+    h = this->screen->h;
+
     SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
 
     SDL_WM_SetCaption("navit", NULL);
@@ -2180,9 +2255,13 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     sge_Lock_ON();
 #endif
 
-	*meth=graphics_methods;
+    *meth=graphics_methods;
 
-    g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
+    if(the_graphics!=NULL) {
+	dbg(0,"graphics_sdl_new: graphics struct already set: %d!\n", the_graphics_count);
+    }
+    the_graphics = this;
+    the_graphics_count++;
 
     this->overlay_enable = 1;
 
@@ -2191,12 +2270,215 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
         this->aa = attr->u.num;
 
     this->resize_callback_initial=1;
+    dbg(1, "leave\n");
     return this;
 }
 
+/* ---------- SDL Eventhandling ---------- */
+
+static Uint32 
+sdl_timer_callback(Uint32 interval, void* param)
+{
+    struct event_timeout *timeout=(struct event_timeout*)param;
+
+    dbg(1,"timer(%p) multi(%d) interval(%d) fired\n", param, timeout->multi, interval);
+
+    SDL_Event event;
+    SDL_UserEvent userevent;
+
+    struct userevent_data *userdata = g_new0(struct userevent_data, 1);
+    userdata->is_idle_event = timeout->multi == 2 ? 1 : 0;
+
+    userevent.type = SDL_USEREVENT;
+    userevent.code = 123;
+    userevent.data1 = timeout->cb;
+    userevent.data2 = userdata;
+ 
+    event.type = SDL_USEREVENT;
+    event.user = userevent;
+     
+    SDL_PushEvent (&event);
+
+    if (timeout->multi!=1) {
+    	g_free(timeout);
+	return 0; // cancel timer
+    }
+    return interval; // reactivate timer
+}
+
+/* SDL Mainloop */
+
+static void
+event_sdl_main_loop_run(void)
+{
+    dbg(1,"enter\n");
+    while(quit_event_loop==0) {
+	graphics_sdl_idle(NULL);
+    }
+}
+
+static void
+event_sdl_main_loop_quit(void)
+{
+    PDL_ScreenTimeoutEnable(PDL_TRUE);
+    PDL_Quit();
+    dbg(1,"enter\n");
+}
+
+/* Watch */
+
+static struct event_watch *
+event_sdl_add_watch(void *h, enum event_watch_cond cond, struct callback *cb)
+{
+    dbg(1,"enter\n");
+    return NULL;
+}
+
+static void
+event_sdl_remove_watch(struct event_watch *ev)
+{
+    dbg(1,"enter %p\n",ev);
+}
+
+/* Timeout */
+
+static struct event_timeout *
+event_sdl_add_timeout(int timeout, int multi, struct callback *cb)
+{
+    struct event_timeout * ret =  g_new0(struct event_timeout, 1);
+    if(!ret)
+	return ret;
+    ret->multi = multi;
+    ret->cb = cb;
+    ret->id = SDL_AddTimer(timeout, sdl_timer_callback, ret);
+    dbg(1,"timer(%p) multi(%d) interval(%d) cb(%p) added\n",ret, multi, timeout, cb);
+    return ret;
+}
+
+static void
+event_sdl_remove_timeout(struct event_timeout *to)
+{
+    dbg(2,"enter %p\n", to);
+    if(to!=NULL)
+    {
+        if (SDL_RemoveTimer(to->id) == SDL_FALSE)
+	    dbg(0,"SDL_RemoveTimer (%p) failed\n", to->id);
+	else {
+            g_free(to);
+            dbg(1,"timer(%p) removed\n", to);
+	}
+    }
+}
+
+/* Idle */
+
+static void
+sdl_idle_callback_enqueue(void *param, void *userdata) 
+{
+    struct idle_task *task = param;
+    idle_events_pending++;
+    dbg(2,"deploying idle task (%p) %i\n", task->cb, idle_events_pending);
+    event_sdl_add_timeout(1, 2, task->cb);
+}
+
+static void
+sdl_idle_timeout_callback(GPtrArray *idle_callbacks)
+{
+    if (!idle_events_pending) {
+	 g_ptr_array_foreach(idle_callbacks, sdl_idle_callback_enqueue, NULL);
+    }
+    idle_timer = NULL;
+}    
+
+/* sort ptr_array by priority, increasing order */
+gint
+sdl_sort_idle_callbacks(gconstpointer parama, gconstpointer paramb)
+{
+    struct idle_task *a = (struct idle_task *)parama;
+    struct idle_task *b = (struct idle_task *)paramb;
+    if (a->priority < b->priority)
+	return -1;
+    if (a->priority > b->priority)
+	return 1;
+    return 0;
+}
+
+static void
+sdl_idle_timeout_rearm() 
+{
+    struct callback *idle_cb = callback_new_1(callback_cast(sdl_idle_timeout_callback), idle_callbacks);
+
+    if (!idle_timer)
+    	idle_timer = event_sdl_add_timeout(100, 0, idle_cb);
+
+}
+
+static struct event_idle *
+event_sdl_add_idle(int priority, struct callback *cb)
+{
+    dbg(1,"add idle priority(%d) cb(%p)\n", priority, cb);
+    if (!idle_callbacks)
+	idle_callbacks = g_ptr_array_new();
+
+    struct idle_task *task = g_new0(struct idle_task, 1);
+    task->priority = priority;
+    task->cb = cb;
+
+    g_ptr_array_add(idle_callbacks, (gpointer)task);
+    g_ptr_array_sort(idle_callbacks, sdl_sort_idle_callbacks);
+
+    sdl_idle_timeout_rearm();
+
+    return (struct event_idle *)task;
+}
+
+static void
+event_sdl_remove_idle(struct event_idle *task)
+{
+    dbg(1,"remove %p\n", task);
+    g_ptr_array_remove(idle_callbacks, (struct idle_task *)task);
+    if (idle_callbacks->len == 0) {
+	event_remove_timeout(idle_timer);
+	idle_timer = NULL;
+    }
+}
+
+/* callback */
+
+static void
+event_sdl_call_callback(struct callback_list *cb)
+{
+    dbg(1,"enter\n");
+}
+
+static struct event_methods event_sdl_methods = {
+    event_sdl_main_loop_run,
+    event_sdl_main_loop_quit,
+    event_sdl_add_watch,
+    event_sdl_remove_watch,
+    event_sdl_add_timeout,
+    event_sdl_remove_timeout,
+    event_sdl_add_idle,
+    event_sdl_remove_idle,
+    event_sdl_call_callback,
+};
+
+static struct event_priv *
+event_sdl_new(struct event_methods* methods)
+{
+    *methods = event_sdl_methods;
+    return NULL;
+}
+
+/* ---------- SDL Eventhandling ---------- */
+
 void
 plugin_init(void)
 {
-        plugin_register_graphics_type("sdl", graphics_sdl_new);
+    plugin_register_event_type("sdl", event_sdl_new);
+    plugin_register_graphics_type("sdl", graphics_sdl_new);
 }
 
+/* 
+ * vim: sw=4 ts=8
+ * */
