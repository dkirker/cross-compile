diff --git a/navit/navit/graphics/sdl/graphics_sdl.c b/navit/navit/graphics/sdl/graphics_sdl.c
index e8c85de..5e58cae 100644
--- a/navit/navit/graphics/sdl/graphics_sdl.c
+++ b/navit/navit/graphics/sdl/graphics_sdl.c
@@ -34,6 +34,11 @@
 #include <SDL/SDL.h>
 #include <math.h>
 
+#ifdef USE_WEBOS
+# include <PDL.h>
+//# define USE_WEBOS_ACCELEROMETER
+#endif
+
 #define RASTER
 #undef SDL_SGE
 #undef SDL_GFX
@@ -43,8 +48,13 @@
 #define SDL_IMAGE
 #undef LINUX_TOUCHSCREEN
 
+#ifdef USE_WEBOS
+#define DISPLAY_W 0
+#define DISPLAY_H 0
+#else
 #define DISPLAY_W 800
 #define DISPLAY_H 600
+#endif
 
 
 #undef DEBUG
@@ -121,6 +131,12 @@ struct graphics_priv {
     struct navit *nav;
     struct callback_list *cbl;
 
+#ifdef USE_WEBOS_ACCELEROMETER
+    SDL_Joystick *accelerometer;
+    char orientation;
+    int real_w, real_h;
+#endif
+
 #ifdef LINUX_TOUCHSCREEN
     int ts_fd;
     int32_t ts_hit;
@@ -141,6 +157,47 @@ struct graphics_priv {
 
 static int dummy;
 
+#ifdef USE_WEBOS
+#define WEBOS_KEY_SHIFT 0x130
+#define WEBOS_KEY_SYM 0x131
+#define WEBOS_KEY_ORANGE 0x133
+
+#define WEBOS_KEY_MOD_SHIFT 0x1
+#define WEBOS_KEY_MOD_ORANGE 0x2
+#define WEBOS_KEY_MOD_SYM 0x4
+
+#define WEBOS_KEY_MOD_SHIFT_STICKY 0x11
+#define WEBOS_KEY_MOD_ORANGE_STICKY 0x22
+#define WEBOS_KEY_MOD_SYM_STICKY 0x44
+
+#ifdef USE_WEBOS_ACCELEROMETER
+# define WEBOS_ORIENTATION_PORTRAIT 0x0
+# define WEBOS_ORIENTATION_LANDSCAPE 0x1
+#endif
+
+#define SDL_USEREVENT_CODE_TIMER 0x1
+#define SDL_USEREVENT_CODE_CALL_CALLBACK 0x2
+#define SDL_USEREVENT_CODE_IDLE_EVENT 0x4
+#ifdef USE_WEBOS_ACCELEROMETER
+# define SDL_USEREVENT_CODE_ROTATE 0x8
+#endif
+
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
+static int quit_event_loop=0; // quit the main event loop
+static struct graphics_priv* the_graphics=NULL; 
+static int the_graphics_count=0; // count how many graphics objects are created
+static GPtrArray *idle_tasks=NULL;
+#endif 
 
 struct graphics_font_priv {
 #ifdef SDL_TTF
@@ -193,6 +250,12 @@ graphics_destroy(struct graphics_priv *gr)
 #ifdef LINUX_TOUCHSCREEN
         input_ts_exit(gr);
 #endif
+#ifdef USE_WEBOS_ACCELEROMETER
+    	SDL_JoystickClose(gr->accelerometer);
+#endif
+#ifdef USE_WEBOS
+        PDL_Quit();
+#endif
         SDL_Quit();
     }
 
@@ -202,6 +265,9 @@ graphics_destroy(struct graphics_priv *gr)
 /* graphics_font */
 static char *fontfamilies[]={
 	"Liberation Mono",
+#ifdef USE_WEBOS
+	"Prelude",
+#endif
 	"Arial",
 	"DejaVu Sans",
 	"NcrBI4nh",
@@ -1832,6 +1898,43 @@ static int input_ts_exit(struct graphics_priv *gr)
 }
 #endif
 
+#ifdef USE_WEBOS_ACCELEROMETER
+static void
+sdl_accelerometer_handler(void* param) 
+{
+    struct graphics_priv *gr = (struct graphics_priv *)param;
+    int xAxis = SDL_JoystickGetAxis(gr->accelerometer, 0);
+    int yAxis = SDL_JoystickGetAxis(gr->accelerometer, 1);
+    int zAxis = SDL_JoystickGetAxis(gr->accelerometer, 2);
+    char new_orientation;
+
+    if (xAxis < -15000 && yAxis > -7000 && yAxis < 7000)
+	new_orientation = WEBOS_ORIENTATION_LANDSCAPE;
+    else if (yAxis > 15000 && xAxis > -7000 && xAxis < 7000)
+	new_orientation = WEBOS_ORIENTATION_PORTRAIT;
+    else
+	return;
+
+    if (new_orientation != gr->orientation)
+    {
+    	dbg(1,"x(%d) y(%d) z(%d) o(%d)\n",xAxis, yAxis, zAxis, new_orientation);
+	gr->orientation = new_orientation;
+    	
+	SDL_Event event;
+    	SDL_UserEvent userevent;
+
+    	userevent.type = SDL_USEREVENT;
+    	userevent.code = SDL_USEREVENT_CODE_ROTATE;
+    	userevent.data1 = NULL;
+    	userevent.data2 = NULL;
+
+    	event.type = SDL_USEREVENT;
+    	event.user = userevent;
+
+    	SDL_PushEvent (&event);
+    }
+}
+#endif
 
 static gboolean graphics_sdl_idle(void *data)
 {
@@ -1842,8 +1945,21 @@ static gboolean graphics_sdl_idle(void *data)
     struct input_event ie;
     ssize_t ss;
 #endif
-    int ret, key;
-    char keybuf[2];
+    int ret;
+    char key_mod = 0;
+    char keybuf[8];
+
+#ifdef USE_WEBOS
+    if(data==NULL) {
+    	if(the_graphics!=NULL) {
+	    gr = the_graphics;
+	} 
+	else {
+	    dbg(0,"graphics_idle: graphics not set!\n");
+    	    return FALSE;
+	}
+    }
+#endif
 
     /* generate the initial resize callback, so the gui knows W/H
 
@@ -1945,14 +2061,56 @@ static gboolean graphics_sdl_idle(void *data)
     }
 #endif
 
+#ifdef USE_WEBOS_ACCELEROMETER
+    struct callback* accel_cb = callback_new_1(callback_cast(sdl_accelerometer_handler), gr);
+    struct event_timeout* accel_to = event_add_timeout(200, 1, accel_cb);
+#endif
+#ifdef USE_WEBOS
+    unsigned int idle_tasks_idx=0;
+    unsigned int idle_tasks_cur_priority;
+    struct idle_task *task;
+
+    while(!quit_event_loop)
+#else
     while(1)
+#endif
     {
+#ifdef USE_WEBOS
+	ret = 0;
+	if(idle_tasks->len > 0) 
+	{
+     	    while (!(ret = SDL_PollEvent(&ev)) && idle_tasks->len > 0)
+	    {
+		if (idle_tasks_idx >= idle_tasks->len)
+		    idle_tasks_idx = 0;
+
+		dbg(3,"idle_tasks_idx(%d)\n",idle_tasks_idx);
+		task = (struct idle_task *)g_ptr_array_index(idle_tasks,idle_tasks_idx);
+		
+		if (idle_tasks_idx == 0)	// only execute tasks with lowest priority value
+		    idle_tasks_cur_priority = task->priority;
+		if (task->priority > idle_tasks_cur_priority)
+		    idle_tasks_idx = 0;
+		else 
+		{
+		    callback_call_0(task->cb);
+		    idle_tasks_idx++;
+		}
+	    }
+	}
+	if (!ret)	// If we get here there are no idle_tasks and we have no events pending
+	    ret = SDL_WaitEvent(&ev);
+#else
         ret = SDL_PollEvent(&ev);
+#endif
         if(ret == 0)
         {
             break;
         }
 
+#ifdef USE_WEBOS
+	dbg(1,"SDL_Event %d\n", ev.type);
+#endif
         switch(ev.type)
         {
             case SDL_MOUSEMOTION:
@@ -1965,59 +2123,124 @@ static gboolean graphics_sdl_idle(void *data)
 
             case SDL_KEYDOWN:
             {
+		keybuf[1] = 0;
                 switch(ev.key.keysym.sym)
                 {
                     case SDLK_LEFT:
                     {
-                        key = NAVIT_KEY_LEFT;
+                        keybuf[0] = NAVIT_KEY_LEFT;
                         break;
                     }
                     case SDLK_RIGHT:
                     {
-                        key = NAVIT_KEY_RIGHT;
+                        keybuf[0] = NAVIT_KEY_RIGHT;
                         break;
                     }
                     case SDLK_BACKSPACE:
                     {
-                        key = NAVIT_KEY_BACKSPACE;
+                        keybuf[0] = NAVIT_KEY_BACKSPACE;
                         break;
                     }
                     case SDLK_RETURN:
                     {
-                        key = NAVIT_KEY_RETURN;
+                        keybuf[0] = NAVIT_KEY_RETURN;
                         break;
                     }
                     case SDLK_DOWN:
                     {
-                        key = NAVIT_KEY_DOWN;
+                        keybuf[0] = NAVIT_KEY_DOWN;
                         break;
                     }
                     case SDLK_PAGEUP:
                     {
-                        key = NAVIT_KEY_ZOOM_OUT;
+                        keybuf[0] = NAVIT_KEY_ZOOM_OUT;
                         break;
                     }
                     case SDLK_UP:
                     {
-                        key = NAVIT_KEY_UP;
+                        keybuf[0] = NAVIT_KEY_UP;
                         break;
                     }
                     case SDLK_PAGEDOWN:
                     {
-                        key = NAVIT_KEY_ZOOM_IN;
+                        keybuf[0] = NAVIT_KEY_ZOOM_IN;
                         break;
                     }
+#ifdef USE_WEBOS
+		    case WEBOS_KEY_SHIFT:
+		    {
+			if ((key_mod & WEBOS_KEY_MOD_SHIFT_STICKY) == WEBOS_KEY_MOD_SHIFT_STICKY)
+			    key_mod &= ~(WEBOS_KEY_MOD_SHIFT_STICKY);
+			else if ((key_mod & WEBOS_KEY_MOD_SHIFT) == WEBOS_KEY_MOD_SHIFT)
+			    key_mod |= WEBOS_KEY_MOD_SHIFT_STICKY;
+			else
+			    key_mod |= WEBOS_KEY_MOD_SHIFT;
+			break;
+		    }
+		    case WEBOS_KEY_ORANGE:
+		    {
+			if ((key_mod & WEBOS_KEY_MOD_ORANGE_STICKY) == WEBOS_KEY_MOD_ORANGE_STICKY)
+			    key_mod &= ~(WEBOS_KEY_MOD_ORANGE_STICKY);
+			else if ((key_mod & WEBOS_KEY_MOD_ORANGE) == WEBOS_KEY_MOD_ORANGE)
+			    key_mod |= WEBOS_KEY_MOD_ORANGE_STICKY;
+			else
+			    key_mod |= WEBOS_KEY_MOD_ORANGE;
+			break;
+		    }
+		    case WEBOS_KEY_SYM:
+		    {
+			/* Toggle the on-screen keyboard */
+			//callback_list_call_attr_1(gr->cbl, attr_keyboard_toggle);	// Not implemented yet
+			break;
+		    }
+#endif
                     default:
                     {
-                        key = 0;
+#ifdef USE_WEBOS
+                        if (ev.key.keysym.unicode < 0x80 && ev.key.keysym.unicode > 0) {
+			    keybuf[0] = (char)ev.key.keysym.unicode;
+			    if ((key_mod & WEBOS_KEY_MOD_ORANGE) == WEBOS_KEY_MOD_ORANGE) {
+				switch(keybuf[0]) {
+				    case 'e': keybuf[0] = '1'; break;
+				    case 'r': keybuf[0] = '2'; break;
+				    case 't': keybuf[0] = '3'; break;
+				    case 'd': keybuf[0] = '4'; break;
+				    case 'f': keybuf[0] = '5'; break;
+				    case 'g': keybuf[0] = '6'; break;
+				    case 'x': keybuf[0] = '7'; break;
+				    case 'c': keybuf[0] = '8'; break;
+				    case 'v': keybuf[0] = '9'; break;
+				    case '@': keybuf[0] = '0'; break;
+				    case ',': keybuf[0] = '-'; break;
+				    case 'u': strncpy(keybuf, "ü", sizeof(keybuf)); break;
+				    case 'a': strncpy(keybuf, "ä", sizeof(keybuf)); break;
+				    case 'o': strncpy(keybuf, "ö", sizeof(keybuf)); break;
+				    case 's': strncpy(keybuf, "ß", sizeof(keybuf)); break;
+				}
+			    }
+			    /*if ((key_mod & WEBOS_KEY_MOD_SHIFT) == WEBOS_KEY_MOD_SHIFT)
+				key -= 32;*/
+			    if ((key_mod & WEBOS_KEY_MOD_SHIFT_STICKY) != WEBOS_KEY_MOD_SHIFT_STICKY)
+				key_mod &= ~(WEBOS_KEY_MOD_SHIFT_STICKY);
+			    if ((key_mod & WEBOS_KEY_MOD_ORANGE_STICKY) != WEBOS_KEY_MOD_ORANGE_STICKY)
+			    	key_mod &= ~(WEBOS_KEY_MOD_ORANGE_STICKY);
+			}
+			else {
+			    dbg(0,"Unknown key sym: %x\n", ev.key.keysym.sym);
+			}
+#else
+			keybuf[0] = 0;
+#endif			
                         break;
                     }
                 }
+			  
+		dbg(2,"key mod: 0x%x\n", key_mod);
 
-                keybuf[0] = key;
-                keybuf[1] = '\0';
-		callback_list_call_attr_1(gr->cbl, attr_keypress, (void *)keybuf);
-
+		if (keybuf[0]) {
+		    dbg(2,"key: %s 0x%x\n", keybuf, keybuf);
+		    callback_list_call_attr_1(gr->cbl, attr_keypress, (void *)keybuf);
+		}
                 break;
             }
 
@@ -2062,6 +2285,9 @@ static gboolean graphics_sdl_idle(void *data)
 
             case SDL_QUIT:
             {
+#ifdef USE_WEBOS
+		quit_event_loop = 1;
+#endif
                 navit_destroy(gr->nav);
                 break;
             }
@@ -2082,6 +2308,55 @@ static gboolean graphics_sdl_idle(void *data)
                 break;
             }
 
+#ifdef USE_WEBOS
+            case SDL_USEREVENT:
+            {
+		SDL_UserEvent userevent = ev.user;
+                if(userevent.type==SDL_USEREVENT && userevent.code==SDL_USEREVENT_CODE_TIMER) 
+		{
+    		    struct callback *cb = (struct callback *)userevent.data1;
+                    dbg(1, "SDL_USEREVENT timer received cb(%p)\n", cb);
+		    callback_call_0(cb);
+                }
+ 		else if(userevent.type==SDL_USEREVENT && userevent.code==SDL_USEREVENT_CODE_CALL_CALLBACK) 
+		{
+    		    struct callback_list *cbl = (struct callback_list *)userevent.data1;
+                    dbg(1, "SDL_USEREVENT call_callback received cbl(%p)\n", cbl);
+		    callback_list_call_0(cbl);
+ 		}
+       		else if(userevent.type==SDL_USEREVENT && userevent.code==SDL_USEREVENT_CODE_IDLE_EVENT) 
+                    dbg(1, "SDL_USEREVENT idle_event received\n");
+#ifdef USE_WEBOS_ACCELEROMETER
+		else if(userevent.type==SDL_USEREVENT && userevent.code==SDL_USEREVENT_CODE_ROTATE)
+		{
+		    dbg(1, "SDL_USEREVENT rotate received\n");
+                    switch(gr->orientation)
+		    {
+			case WEBOS_ORIENTATION_PORTRAIT:
+			    gr->screen = SDL_SetVideoMode(gr->real_w, gr->real_h, gr->video_bpp, gr->video_flags);
+			    PDL_SetOrientation(PDL_ORIENTATION_0);
+			    break;
+			case WEBOS_ORIENTATION_LANDSCAPE:
+			    gr->screen = SDL_SetVideoMode(gr->real_h, gr->real_w, gr->video_bpp, gr->video_flags);
+			    PDL_SetOrientation(PDL_ORIENTATION_270);
+			    break;
+		    }
+                    if(gr->screen == NULL)
+                    {
+                    	navit_destroy(gr->nav);
+                    }
+                    else
+                    {
+		    	callback_list_call_attr_2(gr->cbl, attr_resize, (void *)gr->screen->w, (void *)gr->screen->h);
+                    }
+		}
+#endif
+                else 
+                    dbg(1, "unknown SDL_USEREVENT\n");
+                
+		break;
+            }
+#endif
             default:
             {
 #ifdef DEBUG
@@ -2092,6 +2367,11 @@ static gboolean graphics_sdl_idle(void *data)
         }
     }
 
+#ifdef USE_WEBOS_ACCELEROMETER
+    event_remove_timeout(accel_to);
+    callback_destroy(accel_cb);
+#endif
+ 
     return TRUE;
 }
 
@@ -2107,9 +2387,18 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     this->nav = nav;
     this->cbl = cbl;
 
+#ifdef USE_WEBOS
+# ifdef USE_WEBOS_ACCELEROMETER
+    ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK);
+# else
+    ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
+# endif
+#else
     ret = SDL_Init(SDL_INIT_VIDEO);
+#endif
     if(ret < 0)
     {
+	dbg(0,"SDL_Init failed %d\n", ret);
         g_free(this);
         return NULL;
     }
@@ -2118,7 +2407,11 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     ret = TTF_Init();
     if(ret < 0)
     {
+        dbg(0,"TTF_Init failed %d\n", ret);
         g_free(this);
+#ifdef USE_WEBOS
+        PDL_Quit();
+#endif
         SDL_Quit();
         return NULL;
     }
@@ -2126,11 +2419,22 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     FT_Init_FreeType( &this->library );
 #endif
 
-    if (! event_request_system("glib","graphics_sdl_new"))
+#ifdef USE_WEBOS
+    if (! event_request_system("sdl","graphics_sdl_new")) {
+#else
+    if (! event_request_system("glib","graphics_sdl_new")) {
+#endif
+	dbg(0,"event_request_system failed");
         return NULL;
+    }
 
+#ifdef USE_WEBOS
+    this->video_bpp = 0;
+    this->video_flags = SDL_SWSURFACE | SDL_ANYFORMAT | SDL_RESIZABLE;
+#else
     this->video_bpp = 16;
     this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
+#endif
 
     if ((attr=attr_search(attrs, NULL, attr_w)))
         w=attr->u.num;
@@ -2149,18 +2453,31 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
 
     this->screen = SDL_SetVideoMode(w, h, this->video_bpp, this->video_flags);
 
-    /* Use screen size instead of requested */
-    w = this->screen->w;
-    h = this->screen->h;
-
     if(this->screen == NULL)
     {
+	dbg(0,"SDL_SetVideoMode failed\n");
         g_free(this);
+#ifdef USE_WEBOS
+        PDL_Quit();
+#endif
         SDL_Quit();
         return NULL;
     }
 
+    /* Use screen size instead of requested */
+    w = this->screen->w;
+    h = this->screen->h;
+#ifdef USE_WEBOS_ACCELEROMETER
+    this->real_w = w;
+    this->real_h = h;
+    this->accelerometer = SDL_JoystickOpen(0);
+#endif
+
     SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
+#ifdef USE_WEBOS
+    SDL_EnableUNICODE(1);
+    PDL_SetOrientation(PDL_ORIENTATION_0);
+#endif
 
     SDL_WM_SetCaption("navit", NULL);
 
@@ -2180,9 +2497,17 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     sge_Lock_ON();
 #endif
 
-	*meth=graphics_methods;
+    *meth=graphics_methods;
 
+#ifdef USE_WEBOS
+    if(the_graphics!=NULL) {
+	dbg(0,"graphics_sdl_new: graphics struct already set: %d!\n", the_graphics_count);
+    }
+    the_graphics = this;
+    the_graphics_count++;
+#else
     g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
+#endif
 
     this->overlay_enable = 1;
 
@@ -2194,9 +2519,208 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     return this;
 }
 
+#ifdef USE_WEBOS
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
+    userevent.type = SDL_USEREVENT;
+    userevent.code = SDL_USEREVENT_CODE_TIMER;
+    userevent.data1 = timeout->cb;
+    userevent.data2 = NULL;
+
+    event.type = SDL_USEREVENT;
+    event.user = userevent;
+
+    SDL_PushEvent (&event);
+
+    if (timeout->multi==0) {
+    	g_free(timeout);
+	timeout = NULL;
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
+    PDL_ScreenTimeoutEnable(PDL_FALSE);
+    graphics_sdl_idle(NULL);
+    PDL_ScreenTimeoutEnable(PDL_TRUE);
+}
+
+static void
+event_sdl_main_loop_quit(void)
+{
+    quit_event_loop = 1;
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
+    dbg(1,"timer(%p) multi(%d) interval(%d) cb(%p) added\n",ret, multi, timeout, cb);
+    ret->multi = multi;
+    ret->cb = cb;
+    ret->id = SDL_AddTimer(timeout, sdl_timer_callback, ret);
+
+    return ret;
+}
+
+static void
+event_sdl_remove_timeout(struct event_timeout *to)
+{
+    dbg(2,"enter %p\n", to);
+    if(to!=NULL)
+    {
+	int ret = to->id ? SDL_RemoveTimer(to->id) : SDL_TRUE;
+        if (ret == SDL_FALSE)
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
+/* sort ptr_array by priority, increasing order */
+gint
+sdl_sort_idle_tasks(gconstpointer parama, gconstpointer paramb)
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
+static struct event_idle *
+event_sdl_add_idle(int priority, struct callback *cb)
+{
+    dbg(1,"add idle priority(%d) cb(%p)\n", priority, cb);
+
+    struct idle_task *task = g_new0(struct idle_task, 1);
+    task->priority = priority;
+    task->cb = cb;
+
+    g_ptr_array_add(idle_tasks, (gpointer)task);
+
+    if (idle_tasks->len < 2)
+    {
+    	SDL_Event event;
+    	SDL_UserEvent userevent;
+
+    	dbg(1,"poking eventloop because of new idle_events\n");
+
+    	userevent.type = SDL_USEREVENT;
+    	userevent.code = SDL_USEREVENT_CODE_IDLE_EVENT;
+    	userevent.data1 = NULL;
+    	userevent.data2 = NULL;
+
+    	event.type = SDL_USEREVENT;
+    	event.user = userevent;
+
+    	SDL_PushEvent (&event);
+    }
+    else	// more than one entry => sort the list
+	g_ptr_array_sort(idle_tasks, sdl_sort_idle_tasks);
+
+    return (struct event_idle *)task;
+}
+
+static void
+event_sdl_remove_idle(struct event_idle *task)
+{
+    dbg(1,"remove task(%p)\n", task);
+    g_ptr_array_remove(idle_tasks, (gpointer)task);
+}
+
+/* callback */
+
+static void
+event_sdl_call_callback(struct callback_list *cbl)
+{
+    dbg(1,"call_callback cbl(%p)\n",cbl);
+    SDL_Event event;
+    SDL_UserEvent userevent;
+
+    userevent.type = SDL_USEREVENT;
+    userevent.code = SDL_USEREVENT_CODE_CALL_CALLBACK;
+    userevent.data1 = cbl;
+    userevent.data2 = NULL;
+
+    event.type = SDL_USEREVENT;
+    event.user = userevent;
+
+    SDL_PushEvent (&event);
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
+    idle_tasks = g_ptr_array_new();
+    *methods = event_sdl_methods;
+    return NULL;
+}
+
+/* ---------- SDL Eventhandling ---------- */
+#endif
+
 void
 plugin_init(void)
 {
-        plugin_register_graphics_type("sdl", graphics_sdl_new);
+#ifdef USE_WEBOS
+    plugin_register_event_type("sdl", event_sdl_new);
+#endif
+    plugin_register_graphics_type("sdl", graphics_sdl_new);
 }
 
+/* 
+ * vim: sw=4 ts=8
+ * */
