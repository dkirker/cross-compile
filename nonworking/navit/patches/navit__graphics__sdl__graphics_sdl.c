diff --git a/navit/navit/graphics/sdl/graphics_sdl.c b/navit/navit/graphics/sdl/graphics_sdl.c
index 8e7f561..ad08595 100644
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
@@ -93,6 +94,11 @@
 #include <time.h>
 #endif
 
+extern void* 
+vehicle_webos_new(struct vehicle_methods *meth,
+	       		struct callback_list *cbl,
+		       	struct attr **attrs);
+
 
 /* TODO: union overlay + non-overlay to reduce size */
 struct graphics_priv;
@@ -140,7 +146,16 @@ struct graphics_priv {
 };
 
 static int dummy;
-
+// PRE
+static int    quit_event_loop=0; // quit the main event loop
+static struct graphics_priv* the_graphics=NULL; 
+static int    the_graphics_count=0; // count how many graphics objects are created
+
+struct event_timeout {
+	SDL_TimerID id;
+	int multi;
+	struct callback *cb;
+};
 
 struct graphics_font_priv {
 #ifdef SDL_TTF
@@ -175,7 +190,7 @@ static int input_ts_exit(struct graphics_priv *gr);
 static void
 graphics_destroy(struct graphics_priv *gr)
 {
-    dbg(0, "graphics_destroy %p %u\n", gr, gr->overlay_mode);
+    dbg(1, "enter %p %u\n", gr, gr->overlay_mode);
 
     if(gr->overlay_mode)
     {
@@ -193,10 +208,12 @@ graphics_destroy(struct graphics_priv *gr)
 #ifdef LINUX_TOUCHSCREEN
         input_ts_exit(gr);
 #endif
+        dbg(1, "SDL_Quit\n");
         SDL_Quit();
     }
 
     g_free(gr);
+    dbg(1, "leave\n");
 }
 
 /* graphics_font */
@@ -212,11 +229,13 @@ static char *fontfamilies[]={
 
 static void font_destroy(struct graphics_font_priv *gf)
 {
+    dbg(1, "enter\n");
 #ifdef SDL_TTF
 #else
     FT_Done_Face(gf->face);
 #endif
     g_free(gf);
+    dbg(1, "leave\n");
 }
 
 static struct graphics_font_methods font_methods = {
@@ -225,7 +244,10 @@ static struct graphics_font_methods font_methods = {
 
 static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct graphics_font_methods *meth, char *fontfamily, int size, int flags)
 {
-    struct graphics_font_priv *gf=g_new(struct graphics_font_priv, 1);
+    struct graphics_font_priv *gf;
+
+    dbg(1, "enter\n");
+    gf = g_new(struct graphics_font_priv, 1);
 
 #ifdef SDL_TTF
     /* 'size' is in pixels, TTF_OpenFont wants pts. */
@@ -305,6 +327,7 @@ static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct grap
 	if (!found) {
 		g_warning("Failed to load font, no labelling");
 		g_free(gf);
+                dbg(1, "return\n");
 		return NULL;
 	}
         FT_Set_Char_Size(gf->face, 0, size, 300, 300);
@@ -313,6 +336,7 @@ static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct grap
 
 	*meth=font_methods;
 
+    dbg(1, "leave\n");
     return gf;
 }
 
@@ -322,7 +346,9 @@ static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct grap
 static void
 gc_destroy(struct graphics_gc_priv *gc)
 {
+    dbg(1, "enter\n");
     g_free(gc);
+    dbg(1, "leave\n");
 }
 
 static void
@@ -374,11 +400,14 @@ static struct graphics_gc_methods gc_methods = {
 
 static struct graphics_gc_priv *gc_new(struct graphics_priv *gr, struct graphics_gc_methods *meth)
 {
+    dbg(1, "enter\n");
     struct graphics_gc_priv *gc=g_new0(struct graphics_gc_priv, 1);
 	*meth=gc_methods;
     gc->gr=gr;
     gc->linewidth=1; /* upper layer should override anyway? */
-	return gc;
+    dbg(1, "leave\n");
+    
+    return gc;
 }
 
 
@@ -401,6 +430,7 @@ static struct graphics_image_priv *
 image_new(struct graphics_priv *gr, struct graphics_image_methods *meth, char *name, int *w, int *h,
           struct point *hot, int rotation)
 {
+    dbg(1, "enter\n");
 #ifdef SDL_IMAGE
     struct graphics_image_priv *gi;
 
@@ -427,8 +457,10 @@ image_new(struct graphics_priv *gr, struct graphics_image_methods *meth, char *n
         gi = NULL;
     }
 
+    dbg(1, "leave\n");
     return gi;
 #else
+    dbg(1, "leave\n");
     return NULL;
 #endif
 }
@@ -436,10 +468,12 @@ image_new(struct graphics_priv *gr, struct graphics_image_methods *meth, char *n
 static void
 image_free(struct graphics_priv *gr, struct graphics_image_priv * gi)
 {
+    dbg(1, "enter\n");
 #ifdef SDL_IMAGE
     SDL_FreeSurface(gi->img);
     g_free(gi);
 #endif
+    dbg(1, "leave\n");
 }
 
 static void
@@ -510,6 +544,8 @@ draw_polygon(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point
     Sint16 x, y;
     int i;
 
+    //dbg(1, "enter\n");
+
     vx = alloca(count * sizeof(Sint16));
     vy = alloca(count * sizeof(Sint16));
 
@@ -585,6 +621,7 @@ draw_polygon(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point
                       gc->fore_r, gc->fore_g, gc->fore_b, gc->fore_a);
 #endif
 #endif
+    //dbg(1, "leave\n");
 }
 
 
@@ -592,6 +629,7 @@ draw_polygon(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point
 static void
 draw_rectangle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int w, int h)
 {
+    dbg(1, "enter\n");
 #ifdef DEBUG
         printf("draw_rectangle: %d %d %d %d r=%d g=%d b=%d a=%d\n", p->x, p->y, w, h,
                gc->fore_r, gc->fore_g, gc->fore_b, gc->fore_a);
@@ -635,11 +673,13 @@ draw_rectangle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct poi
 #endif
 #endif
 
+    dbg(1, "leave\n");
 }
 
 static void
 draw_circle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int r)
 {
+    dbg(1, "enter\n");
 #if 0
         if(gc->fore_a != 0xff)
         {
@@ -703,12 +743,14 @@ draw_circle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point
 #endif
 #endif
 #endif
+    dbg(1, "leave\n");
 }
 
 
 static void
 draw_lines(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int count)
 {
+    dbg(1, "enter\n");
     /* you might expect lines to be simpler than the other shapes.
        but, that would be wrong. 1 line can generate 1 polygon + 2 circles
        and even worse, we have to calculate their parameters!
@@ -918,6 +960,7 @@ draw_lines(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *
         }
     }
 #endif
+    dbg(1, "leave\n");
 }
 
 
@@ -929,6 +972,7 @@ draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics
     SDL_Color f, b;
     SDL_Rect r;
 
+    dbg(1, "enter\n");
 #if 0
     /* correct? */
     f.r = bg->back_r;
@@ -958,6 +1002,7 @@ draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics
         //SDL_SetAlpha(ss, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
         SDL_BlitSurface(ss, NULL, gr->screen, &r);
     }
+    dbg(1, "leave\n");
 }
 #else
 
@@ -984,6 +1029,8 @@ display_text_render_shadow(struct text_glyph *g)
 	unsigned char *shadow;
 	unsigned char *p, *pm=g->pixmap;
 
+        dbg(1, "enter\n");
+
 	shadow=malloc(str*(g->h+2));
 	memset(shadow, 0, str*(g->h+2));
 	for (y = 0 ; y < h ; y++) {
@@ -1015,6 +1062,7 @@ display_text_render_shadow(struct text_glyph *g)
 			}
 		}
 	}
+        dbg(1, "leave\n");
 	return shadow;
 }
 
@@ -1031,6 +1079,8 @@ display_text_render(char *text, struct graphics_font_priv *font, int dx, int dy,
 	struct text_glyph *curr;
 	char *p=text;
 
+        dbg(1, "enter\n");
+
 	len=g_utf8_strlen(text, -1);
 	ret=g_malloc(sizeof(*ret)+len*sizeof(struct text_glyph *));
 	ret->glyph_count=len;
@@ -1081,6 +1131,9 @@ display_text_render(char *text, struct graphics_font_priv *font, int dx, int dy,
          	y -= slot->advance.y;
 		p=g_utf8_next_char(p);
 	}
+
+        dbg(1, "leave\n");
+
 	return ret;
 }
 
@@ -1183,6 +1236,7 @@ display_text_draw(struct text_render *text, struct graphics_priv *gr, struct gra
     Uint32 pix;
     Uint8 r, g, b, a;
 
+    dbg(1, "enter\n");
 #if 0
     dbg(0,"%u %u %u %u, %u %u %u %u\n",
         fg->fore_a, fg->fore_r, fg->fore_g, fg->fore_b,
@@ -1272,7 +1326,7 @@ display_text_draw(struct text_render *text, struct graphics_priv *gr, struct gra
                                &b,
                                &a);
 
-#ifdef DEBUG
+#ifdef XXXDEBUG
                     printf("%u %u -> %u off\n",
                            gly->x,
                            gly->y,
@@ -1363,6 +1417,7 @@ display_text_draw(struct text_render *text, struct graphics_priv *gr, struct gra
         }
 	}
     SDL_UnlockSurface(gr->screen);
+    dbg(1, "leave\n");
 }
 
 static void
@@ -1371,6 +1426,7 @@ display_text_free(struct text_render *text)
 	int i;
 	struct text_glyph **gp;
 
+        dbg(1, "enter\n");
 	gp=text->glyph;
 	i=text->glyph_count;
 	while (i-- > 0) {
@@ -1380,12 +1436,14 @@ display_text_free(struct text_render *text)
 		g_free(*gp++);
 	}
 	g_free(text);
+        dbg(1, "leave\n");
 }
 
 static void
 draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics_gc_priv *bg, struct graphics_font_priv *font, char *text, struct point *p, int dx, int dy)
 {
 	struct text_render *t;
+        //dbg(1, "enter\n");
 
 	if (! font)
 		return;
@@ -1406,6 +1464,7 @@ draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics
         	gdk_gc_set_function(bg->gc, GDK_COPY);
 	}
 #endif
+       // dbg(1, "leave\n");
 }
 #endif
 
@@ -1414,6 +1473,7 @@ draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics
 static void
 draw_image(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct point *p, struct graphics_image_priv *img)
 {
+        dbg(1, "enter\n");
 #ifdef SDL_IMAGE
     SDL_Rect r;
 
@@ -1424,6 +1484,7 @@ draw_image(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct point *
 
     SDL_BlitSurface(img->img, NULL, gr->screen, &r);
 #endif
+        dbg(1, "leave\n");
 }
 
 static void
@@ -1460,6 +1521,7 @@ draw_mode(struct graphics_priv *gr, enum draw_mode_num mode)
     SDL_Rect rect;
     int i;
 
+        dbg(1, "enter\n");
     if(gr->overlay_mode)
     {
         /* will be drawn below */
@@ -1514,6 +1576,7 @@ draw_mode(struct graphics_priv *gr, enum draw_mode_num mode)
 
         gr->draw_mode = mode;
     }
+        dbg(1, "leave\n");
 }
 
 static void overlay_disable(struct graphics_priv *gr, int disable)
@@ -1527,6 +1590,7 @@ overlay_new(struct graphics_priv *gr, struct graphics_methods *meth, struct poin
 static int window_fullscreen(struct window *win, int on)
 {
 	struct graphics_priv *gr=(struct graphics_priv *)win->priv;
+        dbg(1, "enter\n");
 
 	/* Update video flags */
 	if(on) {
@@ -1543,12 +1607,14 @@ static int window_fullscreen(struct window *win, int on)
 	else {
 		callback_list_call_attr_2(gr->cbl, attr_resize, (void *)gr->screen->w, (void *)gr->screen->h);
 	}
+        dbg(1, "leave\n");
 	return 1;
 }
 
 static void *
 get_data(struct graphics_priv *this, char *type)
 {
+        dbg(1, "enter\n");
 	if(strcmp(type, "window") == 0) {
 		struct window *win;
 		win=g_new(struct window, 1);
@@ -1559,6 +1625,7 @@ get_data(struct graphics_priv *this, char *type)
 	} else {
 		return &dummy;
 	}
+        dbg(1, "leave\n");
 }
 
 static void draw_drag(struct graphics_priv *gr, struct point *p)
@@ -1603,6 +1670,7 @@ overlay_new(struct graphics_priv *gr, struct graphics_methods *meth, struct poin
 	Uint32 rmask, gmask, bmask, amask;
 	int i;
 
+        dbg(1, "enter\n");
 	for(i = 0; i < OVERLAY_MAX; i++)
 	{
 		if(gr->overlay_array[i] == NULL)
@@ -1670,6 +1738,7 @@ overlay_new(struct graphics_priv *gr, struct graphics_methods *meth, struct poin
 	gr->overlay_array[i] = ov;
 	*meth=graphics_methods;
 
+        dbg(1, "leave\n");
 	return ov;
 }
 
@@ -1845,6 +1914,17 @@ static gboolean graphics_sdl_idle(void *data)
     int ret, key;
     char keybuf[2];
 
+    //printf("in graphics_sdl_idle\n");
+    //Palm Pre mode
+    if(data==NULL) {
+    	if(the_graphics!=NULL) {
+		//printf("graphics_idle: Palm Pre mode\n");
+		gr = the_graphics;
+	} 
+	else {
+		printf("graphics_idle: graphics not set!\n");
+	}
+    }
     /* generate the initial resize callback, so the gui knows W/H
 
        its unsafe to do this directly inside register_resize_callback;
@@ -1950,9 +2030,11 @@ static gboolean graphics_sdl_idle(void *data)
         ret = SDL_PollEvent(&ev);
         if(ret == 0)
         {
+            //printf("SDL_PollEvent: no pending events\n");
             break;
         }
 
+        dbg(1,"SDL_Event %d\n", ev.type);
         switch(ev.type)
         {
             case SDL_MOUSEMOTION:
@@ -2062,6 +2144,7 @@ static gboolean graphics_sdl_idle(void *data)
 
             case SDL_QUIT:
             {
+		quit_event_loop = 1;
                 navit_destroy(gr->nav);
                 break;
             }
@@ -2081,6 +2164,21 @@ static gboolean graphics_sdl_idle(void *data)
 
                 break;
             }
+            case SDL_USEREVENT:
+            {
+#ifdef DEBUG
+                dbg(1, "SDL_USEREVENT received\n");
+#endif
+		SDL_UserEvent userevent = ev.user;
+                if(userevent.type==SDL_USEREVENT && userevent.code==123 && userevent.data2==NULL) {
+                  struct callback *cb = (userevent.data1);
+                  callback_call_0(cb);
+                }
+                else {
+                  dbg(1, "SDL_USEREVENT is not from timer\n");
+                }
+                break;
+            }
 
             default:
             {
@@ -2090,6 +2188,7 @@ static gboolean graphics_sdl_idle(void *data)
                 break;
             }
         }
+        dbg(1, "event processed\n");
     }
 
     return TRUE;
@@ -2099,6 +2198,7 @@ static gboolean graphics_sdl_idle(void *data)
 static struct graphics_priv *
 graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr **attrs, struct callback_list *cbl)
 {
+    dbg(1, "enter\n");
     struct graphics_priv *this=g_new0(struct graphics_priv, 1);
     struct attr *attr;
     int ret;
@@ -2107,17 +2207,22 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     this->nav = nav;
     this->cbl = cbl;
 
-    ret = SDL_Init(SDL_INIT_VIDEO);
+    ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
     if(ret < 0)
     {
+	fprintf(stderr, "SDL_Init failed %d\n", ret);
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
+	fprintf(stderr, "TTF_Init failed %d\n", ret);
         g_free(this);
         SDL_Quit();
         return NULL;
@@ -2126,11 +2231,14 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
     FT_Init_FreeType( &this->library );
 #endif
 
-    if (! event_request_system("glib","graphics_sdl_new"))
+    if (! event_request_system("sdl","graphics_sdl_new")) {
+	fprintf(stderr, "event_request_system failed");
         return NULL;
+    }
 
-    this->video_bpp = 16;
-    this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
+    this->video_bpp = 32;
+    //this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
+    this->video_flags = SDL_SWSURFACE;
 
     if ((attr=attr_search(attrs, NULL, attr_w)))
         w=attr->u.num;
@@ -2144,17 +2252,18 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
 
     this->screen = SDL_SetVideoMode(w, h, this->video_bpp, this->video_flags);
 
-    /* Use screen size instead of requested */
-    w = this->screen->w;
-    h = this->screen->h;
-
     if(this->screen == NULL)
     {
+	fprintf(stderr, "SDL_SetVideoMode failed\n");
         g_free(this);
         SDL_Quit();
         return NULL;
     }
 
+    /* Use screen size instead of requested */
+    w = this->screen->w;
+    h = this->screen->h;
+
     SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
 
     SDL_WM_SetCaption("navit", NULL);
@@ -2177,7 +2286,14 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
 
 	*meth=graphics_methods;
 
-    g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
+    /*  Will be called by the sdl event loop
+	g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
+    */
+    if(the_graphics!=NULL) {
+	printf("graphics_sdl_new: graphics struct already set: %d!\n", the_graphics_count);
+    }
+    the_graphics = this;
+    the_graphics_count++;
 
     this->overlay_enable = 1;
 
@@ -2186,12 +2302,136 @@ graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr *
         this->aa = attr->u.num;
 
     this->resize_callback_initial=1;
+    dbg(1, "leave\n");
     return this;
 }
 
+static Uint32 sdl_timer_callback(Uint32 interval, void* param)
+{
+    struct event_timeout *timeout=(struct event_timeout*)param;
+
+    dbg(1,"timer fired timer=%p\n", param);
+
+    SDL_Event event;
+    SDL_UserEvent userevent;
+ 
+    userevent.type = SDL_USEREVENT;
+    userevent.code = 123;
+    userevent.data1 = timeout->cb;
+    userevent.data2 = NULL; 
+ 
+    event.type = SDL_USEREVENT;
+    event.user = userevent;
+ 
+    SDL_PushEvent (&event);
+
+    //callback_call_0(timeout->cb);
+
+    if(timeout->multi==0) {
+    	g_free(timeout);
+	return 0; // cancel timer
+    }
+    return interval; // reactivate timer
+}
+
+static void
+event_sdl_main_loop_run(void)
+{
+        dbg(0,"enter\n");
+	while(quit_event_loop==0) {
+		graphics_sdl_idle(NULL);
+	}
+}
+
+static void
+event_sdl_main_loop_quit(void)
+{
+	PDL_ScreenTimeoutEnable(PDL_TRUE);
+	//PDL_Quit();
+        dbg(0,"enter\n");
+}
+
+static struct event_watch *
+event_sdl_add_watch(void *h, enum event_watch_cond cond, struct callback *cb)
+{
+        dbg(0,"enter\n");
+	return NULL;
+}
+
+static void
+event_sdl_remove_watch(struct event_watch *ev)
+{
+	dbg(0,"enter %p\n",ev);
+}
+
+static struct event_timeout *
+event_sdl_add_timeout(int timeout, int multi, struct callback *cb)
+{
+	struct event_timeout * ret =  g_new0(struct event_timeout, 1);
+	if(!ret)
+		return ret;
+	ret->multi = multi;
+        ret->cb = cb;
+	ret->id = SDL_AddTimer(timeout, sdl_timer_callback, ret);
+        dbg(1,"timer multi(%d) interval(%d) %p added\n", multi, timeout, ret);
+	return ret;
+}
+
+static void
+event_sdl_remove_timeout(struct event_timeout *to)
+{
+    dbg(1,"enter %p\n", to);
+    if(to!=NULL)
+    {
+        SDL_RemoveTimer(to->id);
+        g_free(to);
+        dbg(1,"timer %p removed\n", to);
+    }
+}
+
+static struct event_idle *
+event_sdl_add_idle(int priority, struct callback *cb)
+{
+    dbg(1,"enter priority %d %p\n", priority, cb);
+    return (struct event_idle *)event_sdl_add_timeout(1, 1, cb);
+}
+
+static void
+event_sdl_remove_idle(struct event_idle *ev)
+{
+    dbg(1,"enter %p\n", ev);
+    event_sdl_remove_timeout((struct event_timeout *)ev);
+}
+
+static void
+event_sdl_call_callback(struct callback_list *cb)
+{
+    dbg(0,"enter\n");
+}
+
+static struct event_methods event_sdl_methods = {
+        event_sdl_main_loop_run,
+        event_sdl_main_loop_quit,
+        event_sdl_add_watch,
+        event_sdl_remove_watch,
+        event_sdl_add_timeout,
+        event_sdl_remove_timeout,
+        event_sdl_add_idle,
+        event_sdl_remove_idle,
+        event_sdl_call_callback,
+};
+
+static struct event_priv *
+event_sdl_new(struct event_methods* methods)
+{
+	*methods = event_sdl_methods;
+	return NULL;
+}
+
 void
 plugin_init(void)
 {
         plugin_register_graphics_type("sdl", graphics_sdl_new);
+	plugin_register_event_type("sdl", event_sdl_new);
 }
 
