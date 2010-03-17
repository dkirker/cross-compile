*** build/src/navit/graphics/sdl/graphics_sdl.c	2010-03-17 20:43:57.000000000 +0100
--- graphics_sdl.c	2010-03-17 20:42:41.000000000 +0100
***************
*** 33,38 ****
--- 33,39 ----
  
  #include <SDL/SDL.h>
  #include <math.h>
+ #include <PDL.h>
  
  #define RASTER
  #undef SDL_SGE
***************
*** 43,50 ****
  #define SDL_IMAGE
  #undef LINUX_TOUCHSCREEN
  
! #define DISPLAY_W 800
! #define DISPLAY_H 600
  
  
  #undef DEBUG
--- 44,51 ----
  #define SDL_IMAGE
  #undef LINUX_TOUCHSCREEN
  
! #define DISPLAY_W 320
! #define DISPLAY_H 480
  
  
  #undef DEBUG
***************
*** 93,98 ****
--- 94,104 ----
  #include <time.h>
  #endif
  
+ extern void* 
+ vehicle_webos_new(struct vehicle_methods *meth,
+ 	       		struct callback_list *cbl,
+ 		       	struct attr **attrs);
+ 
  
  /* TODO: union overlay + non-overlay to reduce size */
  struct graphics_priv;
***************
*** 140,146 ****
  };
  
  static int dummy;
! 
  
  struct graphics_font_priv {
  #ifdef SDL_TTF
--- 146,161 ----
  };
  
  static int dummy;
! // PRE
! static int    quit_event_loop=0; // quit the main event loop
! static struct graphics_priv* the_graphics=NULL; 
! static int    the_graphics_count=0; // count how many graphics objects are created
! 
! struct event_timeout {
! 	SDL_TimerID id;
! 	int multi;
! 	struct callback *cb;
! };
  
  struct graphics_font_priv {
  #ifdef SDL_TTF
***************
*** 175,181 ****
  static void
  graphics_destroy(struct graphics_priv *gr)
  {
!     dbg(0, "graphics_destroy %p %u\n", gr, gr->overlay_mode);
  
      if(gr->overlay_mode)
      {
--- 190,196 ----
  static void
  graphics_destroy(struct graphics_priv *gr)
  {
!     dbg(0, "enter %p %u\n", gr, gr->overlay_mode);
  
      if(gr->overlay_mode)
      {
***************
*** 193,202 ****
--- 208,219 ----
  #ifdef LINUX_TOUCHSCREEN
          input_ts_exit(gr);
  #endif
+         dbg(0, "SDL_Quit\n");
          SDL_Quit();
      }
  
      g_free(gr);
+     dbg(0, "leave\n");
  }
  
  /* graphics_font */
***************
*** 212,222 ****
--- 229,241 ----
  
  static void font_destroy(struct graphics_font_priv *gf)
  {
+     dbg(0, "enter\n");
  #ifdef SDL_TTF
  #else
      FT_Done_Face(gf->face);
  #endif
      g_free(gf);
+     dbg(0, "leave\n");
  }
  
  static struct graphics_font_methods font_methods = {
***************
*** 225,231 ****
  
  static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct graphics_font_methods *meth, char *fontfamily, int size, int flags)
  {
!     struct graphics_font_priv *gf=g_new(struct graphics_font_priv, 1);
  
  #ifdef SDL_TTF
      /* 'size' is in pixels, TTF_OpenFont wants pts. */
--- 244,253 ----
  
  static struct graphics_font_priv *font_new(struct graphics_priv *gr, struct graphics_font_methods *meth, char *fontfamily, int size, int flags)
  {
!     struct graphics_font_priv *gf;
! 
!     dbg(0, "enter\n");
!     gf = g_new(struct graphics_font_priv, 1);
  
  #ifdef SDL_TTF
      /* 'size' is in pixels, TTF_OpenFont wants pts. */
***************
*** 305,310 ****
--- 327,333 ----
  	if (!found) {
  		g_warning("Failed to load font, no labelling");
  		g_free(gf);
+                 dbg(0, "return\n");
  		return NULL;
  	}
          FT_Set_Char_Size(gf->face, 0, size, 300, 300);
***************
*** 313,318 ****
--- 336,342 ----
  
  	*meth=font_methods;
  
+     dbg(0, "leave\n");
      return gf;
  }
  
***************
*** 322,328 ****
--- 346,354 ----
  static void
  gc_destroy(struct graphics_gc_priv *gc)
  {
+     dbg(0, "enter\n");
      g_free(gc);
+     dbg(0, "leave\n");
  }
  
  static void
***************
*** 374,384 ****
  
  static struct graphics_gc_priv *gc_new(struct graphics_priv *gr, struct graphics_gc_methods *meth)
  {
      struct graphics_gc_priv *gc=g_new0(struct graphics_gc_priv, 1);
  	*meth=gc_methods;
      gc->gr=gr;
      gc->linewidth=1; /* upper layer should override anyway? */
! 	return gc;
  }
  
  
--- 400,413 ----
  
  static struct graphics_gc_priv *gc_new(struct graphics_priv *gr, struct graphics_gc_methods *meth)
  {
+     dbg(0, "enter\n");
      struct graphics_gc_priv *gc=g_new0(struct graphics_gc_priv, 1);
  	*meth=gc_methods;
      gc->gr=gr;
      gc->linewidth=1; /* upper layer should override anyway? */
!     dbg(0, "leave\n");
!     
!     return gc;
  }
  
  
***************
*** 401,406 ****
--- 430,436 ----
  image_new(struct graphics_priv *gr, struct graphics_image_methods *meth, char *name, int *w, int *h,
            struct point *hot, int rotation)
  {
+     dbg(0, "enter\n");
  #ifdef SDL_IMAGE
      struct graphics_image_priv *gi;
  
***************
*** 427,434 ****
--- 457,466 ----
          gi = NULL;
      }
  
+     dbg(0, "leave\n");
      return gi;
  #else
+     dbg(0, "leave\n");
      return NULL;
  #endif
  }
***************
*** 436,445 ****
--- 468,479 ----
  static void
  image_free(struct graphics_priv *gr, struct graphics_image_priv * gi)
  {
+     dbg(0, "enter\n");
  #ifdef SDL_IMAGE
      SDL_FreeSurface(gi->img);
      g_free(gi);
  #endif
+     dbg(0, "leave\n");
  }
  
  static void
***************
*** 510,515 ****
--- 544,551 ----
      Sint16 x, y;
      int i;
  
+     //dbg(0, "enter\n");
+ 
      vx = alloca(count * sizeof(Sint16));
      vy = alloca(count * sizeof(Sint16));
  
***************
*** 585,590 ****
--- 621,627 ----
                        gc->fore_r, gc->fore_g, gc->fore_b, gc->fore_a);
  #endif
  #endif
+     //dbg(0, "leave\n");
  }
  
  
***************
*** 592,597 ****
--- 629,635 ----
  static void
  draw_rectangle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int w, int h)
  {
+     dbg(0, "enter\n");
  #ifdef DEBUG
          printf("draw_rectangle: %d %d %d %d r=%d g=%d b=%d a=%d\n", p->x, p->y, w, h,
                 gc->fore_r, gc->fore_g, gc->fore_b, gc->fore_a);
***************
*** 635,645 ****
--- 673,685 ----
  #endif
  #endif
  
+     dbg(0, "leave\n");
  }
  
  static void
  draw_circle(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int r)
  {
+     dbg(0, "enter\n");
  #if 0
          if(gc->fore_a != 0xff)
          {
***************
*** 703,714 ****
--- 743,756 ----
  #endif
  #endif
  #endif
+     dbg(0, "leave\n");
  }
  
  
  static void
  draw_lines(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int count)
  {
+     dbg(0, "enter\n");
      /* you might expect lines to be simpler than the other shapes.
         but, that would be wrong. 1 line can generate 1 polygon + 2 circles
         and even worse, we have to calculate their parameters!
***************
*** 918,923 ****
--- 960,966 ----
          }
      }
  #endif
+     dbg(0, "leave\n");
  }
  
  
***************
*** 929,934 ****
--- 972,978 ----
      SDL_Color f, b;
      SDL_Rect r;
  
+     dbg(0, "enter\n");
  #if 0
      /* correct? */
      f.r = bg->back_r;
***************
*** 958,963 ****
--- 1002,1008 ----
          //SDL_SetAlpha(ss, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
          SDL_BlitSurface(ss, NULL, gr->screen, &r);
      }
+     dbg(0, "leave\n");
  }
  #else
  
***************
*** 984,989 ****
--- 1029,1036 ----
  	unsigned char *shadow;
  	unsigned char *p, *pm=g->pixmap;
  
+         dbg(0, "enter\n");
+ 
  	shadow=malloc(str*(g->h+2));
  	memset(shadow, 0, str*(g->h+2));
  	for (y = 0 ; y < h ; y++) {
***************
*** 1015,1020 ****
--- 1062,1068 ----
  			}
  		}
  	}
+         dbg(0, "leave\n");
  	return shadow;
  }
  
***************
*** 1031,1036 ****
--- 1079,1086 ----
  	struct text_glyph *curr;
  	char *p=text;
  
+         dbg(0, "enter\n");
+ 
  	len=g_utf8_strlen(text, -1);
  	ret=g_malloc(sizeof(*ret)+len*sizeof(struct text_glyph *));
  	ret->glyph_count=len;
***************
*** 1081,1086 ****
--- 1131,1139 ----
           	y -= slot->advance.y;
  		p=g_utf8_next_char(p);
  	}
+ 
+         dbg(0, "leave\n");
+ 
  	return ret;
  }
  
***************
*** 1183,1188 ****
--- 1236,1242 ----
      Uint32 pix;
      Uint8 r, g, b, a;
  
+     dbg(0, "enter\n");
  #if 0
      dbg(0,"%u %u %u %u, %u %u %u %u\n",
          fg->fore_a, fg->fore_r, fg->fore_g, fg->fore_b,
***************
*** 1272,1278 ****
                                 &b,
                                 &a);
  
! #ifdef DEBUG
                      printf("%u %u -> %u off\n",
                             gly->x,
                             gly->y,
--- 1326,1332 ----
                                 &b,
                                 &a);
  
! #ifdef XXXDEBUG
                      printf("%u %u -> %u off\n",
                             gly->x,
                             gly->y,
***************
*** 1363,1368 ****
--- 1417,1423 ----
          }
  	}
      SDL_UnlockSurface(gr->screen);
+     dbg(0, "leave\n");
  }
  
  static void
***************
*** 1371,1376 ****
--- 1426,1432 ----
  	int i;
  	struct text_glyph **gp;
  
+         dbg(0, "enter\n");
  	gp=text->glyph;
  	i=text->glyph_count;
  	while (i-- > 0) {
***************
*** 1380,1391 ****
--- 1436,1449 ----
  		g_free(*gp++);
  	}
  	g_free(text);
+         dbg(0, "leave\n");
  }
  
  static void
  draw_text(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics_gc_priv *bg, struct graphics_font_priv *font, char *text, struct point *p, int dx, int dy)
  {
  	struct text_render *t;
+         //dbg(0, "enter\n");
  
  	if (! font)
  		return;
***************
*** 1406,1411 ****
--- 1464,1470 ----
          	gdk_gc_set_function(bg->gc, GDK_COPY);
  	}
  #endif
+        // dbg(0, "leave\n");
  }
  #endif
  
***************
*** 1414,1419 ****
--- 1473,1479 ----
  static void
  draw_image(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct point *p, struct graphics_image_priv *img)
  {
+         dbg(0, "enter\n");
  #ifdef SDL_IMAGE
      SDL_Rect r;
  
***************
*** 1424,1429 ****
--- 1484,1490 ----
  
      SDL_BlitSurface(img->img, NULL, gr->screen, &r);
  #endif
+         dbg(0, "leave\n");
  }
  
  static void
***************
*** 1460,1465 ****
--- 1521,1527 ----
      SDL_Rect rect;
      int i;
  
+         dbg(0, "enter\n");
      if(gr->overlay_mode)
      {
          /* will be drawn below */
***************
*** 1514,1519 ****
--- 1576,1582 ----
  
          gr->draw_mode = mode;
      }
+         dbg(0, "leave\n");
  }
  
  static void overlay_disable(struct graphics_priv *gr, int disable)
***************
*** 1527,1532 ****
--- 1590,1596 ----
  static int window_fullscreen(struct window *win, int on)
  {
  	struct graphics_priv *gr=(struct graphics_priv *)win->priv;
+         dbg(0, "enter\n");
  
  	/* Update video flags */
  	if(on) {
***************
*** 1543,1554 ****
--- 1607,1620 ----
  	else {
  		callback_list_call_attr_2(gr->cbl, attr_resize, (void *)gr->screen->w, (void *)gr->screen->h);
  	}
+         dbg(0, "leave\n");
  	return 1;
  }
  
  static void *
  get_data(struct graphics_priv *this, char *type)
  {
+         dbg(0, "enter\n");
  	if(strcmp(type, "window") == 0) {
  		struct window *win;
  		win=g_new(struct window, 1);
***************
*** 1559,1564 ****
--- 1625,1631 ----
  	} else {
  		return &dummy;
  	}
+         dbg(0, "leave\n");
  }
  
  static void draw_drag(struct graphics_priv *gr, struct point *p)
***************
*** 1603,1608 ****
--- 1670,1676 ----
  	Uint32 rmask, gmask, bmask, amask;
  	int i;
  
+         dbg(0, "enter\n");
  	for(i = 0; i < OVERLAY_MAX; i++)
  	{
  		if(gr->overlay_array[i] == NULL)
***************
*** 1670,1675 ****
--- 1738,1744 ----
  	gr->overlay_array[i] = ov;
  	*meth=graphics_methods;
  
+         dbg(0, "leave\n");
  	return ov;
  }
  
***************
*** 1845,1850 ****
--- 1914,1930 ----
      int ret, key;
      char keybuf[2];
  
+     //printf("in graphics_sdl_idle\n");
+     //Palm Pre mode
+     if(data==NULL) {
+     	if(the_graphics!=NULL) {
+ 		//printf("graphics_idle: Palm Pre mode\n");
+ 		gr = the_graphics;
+ 	} 
+ 	else {
+ 		printf("graphics_idle: graphics not set!\n");
+ 	}
+     }
      /* generate the initial resize callback, so the gui knows W/H
  
         its unsafe to do this directly inside register_resize_callback;
***************
*** 1950,1958 ****
--- 2030,2040 ----
          ret = SDL_PollEvent(&ev);
          if(ret == 0)
          {
+             //printf("SDL_PollEvent: no pending events\n");
              break;
          }
  
+         dbg(0,"SDL_Event %d\n", ev.type);
          switch(ev.type)
          {
              case SDL_MOUSEMOTION:
***************
*** 2062,2067 ****
--- 2144,2150 ----
  
              case SDL_QUIT:
              {
+ 		quit_event_loop = 1;
                  navit_destroy(gr->nav);
                  break;
              }
***************
*** 2081,2086 ****
--- 2164,2184 ----
  
                  break;
              }
+             case SDL_USEREVENT:
+             {
+ #ifdef DEBUG
+                 dbg(0, "SDL_USEREVENT received\n");
+ #endif
+ 		SDL_UserEvent userevent = ev.user;
+                 if(userevent.type==SDL_USEREVENT && userevent.code==123 && userevent.data2==NULL) {
+                   struct callback *cb = (userevent.data1);
+                   callback_call_0(cb);
+                 }
+                 else {
+                   dbg(0, "SDL_USEREVENT is not from timer\n");
+                 }
+                 break;
+             }
  
              default:
              {
***************
*** 2090,2095 ****
--- 2188,2194 ----
                  break;
              }
          }
+         dbg(0, "event processed\n");
      }
  
      return TRUE;
***************
*** 2099,2123 ****
  static struct graphics_priv *
  graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr **attrs, struct callback_list *cbl)
  {
      struct graphics_priv *this=g_new0(struct graphics_priv, 1);
      struct attr *attr;
      int ret;
      int w=DISPLAY_W,h=DISPLAY_H;
  
      this->nav = nav;
      this->cbl = cbl;
  
!     ret = SDL_Init(SDL_INIT_VIDEO);
      if(ret < 0)
      {
          g_free(this);
          return NULL;
      }
  
  #ifdef SDL_TTF
      ret = TTF_Init();
      if(ret < 0)
      {
          g_free(this);
          SDL_Quit();
          return NULL;
--- 2198,2229 ----
  static struct graphics_priv *
  graphics_sdl_new(struct navit *nav, struct graphics_methods *meth, struct attr **attrs, struct callback_list *cbl)
  {
+     dbg(0, "enter\n");
      struct graphics_priv *this=g_new0(struct graphics_priv, 1);
      struct attr *attr;
      int ret;
+     PDL_Err pdl_err;
      int w=DISPLAY_W,h=DISPLAY_H;
  
      this->nav = nav;
      this->cbl = cbl;
  
!     ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
      if(ret < 0)
      {
+ 	fprintf(stderr, "SDL_Init failed %d\n", ret);
          g_free(this);
          return NULL;
      }
  
+     PDL_SetOrientation(PDL_ORIENTATION_0);
+     PDL_ScreenTimeoutEnable(PDL_FALSE);
+ 
  #ifdef SDL_TTF
      ret = TTF_Init();
      if(ret < 0)
      {
+ 	fprintf(stderr, "TTF_Init failed %d\n", ret);
          g_free(this);
          SDL_Quit();
          return NULL;
***************
*** 2126,2136 ****
      FT_Init_FreeType( &this->library );
  #endif
  
!     if (! event_request_system("glib","graphics_sdl_new"))
          return NULL;
  
!     this->video_bpp = 16;
!     this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
  
      if ((attr=attr_search(attrs, NULL, attr_w)))
          w=attr->u.num;
--- 2232,2245 ----
      FT_Init_FreeType( &this->library );
  #endif
  
!     if (! event_request_system("sdl","graphics_sdl_new")) {
! 	fprintf(stderr, "event_request_system failed");
          return NULL;
+     }
  
!     this->video_bpp = 32;
!     //this->video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
!     this->video_flags = SDL_SWSURFACE;
  
      if ((attr=attr_search(attrs, NULL, attr_w)))
          w=attr->u.num;
***************
*** 2144,2160 ****
  
      this->screen = SDL_SetVideoMode(w, h, this->video_bpp, this->video_flags);
  
-     /* Use screen size instead of requested */
-     w = this->screen->w;
-     h = this->screen->h;
- 
      if(this->screen == NULL)
      {
          g_free(this);
          SDL_Quit();
          return NULL;
      }
  
      SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
      SDL_WM_SetCaption("navit", NULL);
--- 2253,2270 ----
  
      this->screen = SDL_SetVideoMode(w, h, this->video_bpp, this->video_flags);
  
      if(this->screen == NULL)
      {
+ 	fprintf(stderr, "SDL_SetVideoMode failed\n");
          g_free(this);
          SDL_Quit();
          return NULL;
      }
  
+     /* Use screen size instead of requested */
+     w = this->screen->w;
+     h = this->screen->h;
+ 
      SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
      SDL_WM_SetCaption("navit", NULL);
***************
*** 2177,2183 ****
  
  	*meth=graphics_methods;
  
!     g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
  
      this->overlay_enable = 1;
  
--- 2287,2300 ----
  
  	*meth=graphics_methods;
  
!     /*  Will be called by the sdl event loop
! 	g_timeout_add(G_PRIORITY_DEFAULT+10, graphics_sdl_idle, this);
!     */
!     if(the_graphics!=NULL) {
! 	printf("graphics_sdl_new: graphics struct already set: %d!\n", the_graphics_count);
!     }
!     the_graphics = this;
!     the_graphics_count++;
  
      this->overlay_enable = 1;
  
***************
*** 2186,2197 ****
--- 2303,2439 ----
          this->aa = attr->u.num;
  
      this->resize_callback_initial=1;
+     dbg(0, "leave\n");
      return this;
  }
  
+ static Uint32 sdl_timer_callback(Uint32 interval, void* param)
+ {
+     struct event_timeout *timeout=(struct event_timeout*)param;
+ 
+     dbg(0,"timer fired timer=%p\n", param);
+ 
+     SDL_Event event;
+     SDL_UserEvent userevent;
+  
+     userevent.type = SDL_USEREVENT;
+     userevent.code = 123;
+     userevent.data1 = timeout->cb;
+     userevent.data2 = NULL; 
+  
+     event.type = SDL_USEREVENT;
+     event.user = userevent;
+  
+     SDL_PushEvent (&event);
+ 
+     //callback_call_0(timeout->cb);
+ 
+     if(timeout->multi==0) {
+     	g_free(timeout);
+ 	return 0; // cancel timer
+     }
+     return interval; // reactivate timer
+ }
+ 
+ static void
+ event_sdl_main_loop_run(void)
+ {
+         dbg(0,"enter\n");
+ 	while(quit_event_loop==0) {
+ 		graphics_sdl_idle(NULL);
+ 	}
+ }
+ 
+ static void
+ event_sdl_main_loop_quit(void)
+ {
+         PDL_ScreenTimeoutEnable(PDL_TRUE);
+ 	PDL_Quit();
+         dbg(0,"enter\n");
+ }
+ 
+ static struct event_watch *
+ event_sdl_add_watch(void *h, enum event_watch_cond cond, struct callback *cb)
+ {
+         dbg(0,"enter\n");
+ 	return NULL;
+ }
+ 
+ static void
+ event_sdl_remove_watch(struct event_watch *ev)
+ {
+ 	dbg(0,"enter %p\n",ev);
+ }
+ 
+ static struct event_timeout *
+ event_sdl_add_timeout(int timeout, int multi, struct callback *cb)
+ {
+ 	struct event_timeout * ret =  g_new0(struct event_timeout, 1);
+ 	if(!ret)
+ 		return ret;
+ 	ret->multi = multi;
+         ret->cb = cb;
+ 	ret->id = SDL_AddTimer(timeout, sdl_timer_callback, ret);
+         dbg(0,"timer multi(%d) interval(%d) %p added\n", multi, timeout, ret);
+ 	return ret;
+ }
+ 
+ static void
+ event_sdl_remove_timeout(struct event_timeout *to)
+ {
+     dbg(0,"enter %p\n", to);
+     if(to!=NULL)
+     {
+         SDL_RemoveTimer(to->id);
+         g_free(to);
+         dbg(0,"timer %p removed\n", to);
+     }
+ }
+ 
+ static struct event_idle *
+ event_sdl_add_idle(int priority, struct callback *cb)
+ {
+     dbg(0,"enter priority %d %p\n", priority, cb);
+     return (struct event_idle *)event_sdl_add_timeout(1, 1, cb);
+ }
+ 
+ static void
+ event_sdl_remove_idle(struct event_idle *ev)
+ {
+     dbg(0,"enter %p\n", ev);
+     event_sdl_remove_timeout((struct event_timeout *)ev);
+ }
+ 
+ static void
+ event_sdl_call_callback(struct callback_list *cb)
+ {
+     dbg(0,"enter\n");
+ }
+ 
+ static struct event_methods event_sdl_methods = {
+         event_sdl_main_loop_run,
+         event_sdl_main_loop_quit,
+         event_sdl_add_watch,
+         event_sdl_remove_watch,
+         event_sdl_add_timeout,
+         event_sdl_remove_timeout,
+         event_sdl_add_idle,
+         event_sdl_remove_idle,
+         event_sdl_call_callback,
+ };
+ 
+ static struct event_priv *
+ event_sdl_new(struct event_methods* methods)
+ {
+ 	*methods = event_sdl_methods;
+ 	return NULL;
+ }
+ 
  void
  plugin_init(void)
  {
          plugin_register_graphics_type("sdl", graphics_sdl_new);
+ 	plugin_register_event_type("sdl", event_sdl_new);
+ //	plugin_register_vehicle_type("webos", vehicle_webos_new);
  }
  
