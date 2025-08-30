/*
**                                Xgraphics
**                                ---------
**
**  Version 1.00 ( 28/04/96 )
**
**  Copyright (C) 1996  Martin Lueders (lueders@physik.uni-wuerzburg.de)
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include "Xgraphics.h"

# if defined(__cplusplus) || defined(c_plusplus)
#   define get_class(pvisual)   pvisual -> c_class
#else
#   define get_class(pvisual)   pvisual -> class
#endif


Display         *mydisplay;
int             myscreen;
Window          mywindow;
World           myworld;
unsigned long   black,white,global_color;
int             global_function;
GC              mygc,cleargc,initgc;
XEvent          myevent;
XSetWindowAttributes attributes;
int             buttonflag;
ButtonList      buttonlist;
unsigned long   mycolors[MAX_COLORS];
int             NofColors;
Colormap        mycmap;

world_node_t*  make_world_node();
world_node_t*  find_world_node( World world );

win_node_t*  worldlist;
win_node_t*  make_win_node();
win_node_t*  find_win_node( Window window );



 
void InitX()
{
  char **fontlist;
  int  i,count;
  Font myfont;
  char myfontpattern[40];
  XColor col, colexact;

  /* set display to value of environment DISPLAY */
  
  mydisplay=XOpenDisplay("");
  if (mydisplay==NULL) {
    printf("Cannot connect to X Server\n");
    exit(0);
  }
  myscreen=DefaultScreen(mydisplay);
  
  /* set colors */

  mycmap = DefaultColormap(mydisplay,myscreen);
  
  black=BlackPixel(mydisplay,myscreen);
  white=WhitePixel(mydisplay,myscreen);
  global_color = black;
  global_function = GXcopy;

  mygc    = XCreateGC(mydisplay,DefaultRootWindow(mydisplay),0,0);
  cleargc = XCreateGC(mydisplay,DefaultRootWindow(mydisplay),0,0);
  initgc  = XCreateGC(mydisplay,DefaultRootWindow(mydisplay),0,0);
  
  XSetForeground(mydisplay,mygc,black);
  XSetBackground(mydisplay,mygc,white);
  
  XSetForeground(mydisplay,cleargc,white);
  XSetBackground(mydisplay,cleargc,black);
  
  XFlush(mydisplay);
  
  if( DefaultDepth(mydisplay,myscreen) == 1 ) {

    mycolors[0] = white;
    mycolors[1] = black;
    NofColors = 2;

  } else {
    if ( get_class(DefaultVisual(mydisplay,myscreen)) < 2 ) {
      for(i=NofColors=0;i<MAX_COLORS;++i) {
	if (XAllocNamedColor(mydisplay,mycmap,graylist[i],&col,&colexact) )
	  mycolors[NofColors++] = col.pixel; 
      }
    } else {
      for(i=NofColors=0;i<MAX_COLORS;++i) {
	if (XAllocNamedColor(mydisplay,mycmap,colorlist[i],&col,&colexact) )
	  mycolors[NofColors++] = col.pixel;
      }
    }
  }

  /* set default font */

  strcpy(myfontpattern,"-*-helvetica-bold-r-normal--14*");

  fontlist = XListFonts(mydisplay,myfontpattern,20,&count);
  if(count) {
    myfont=XLoadFont(mydisplay,myfontpattern);
    XSetFont(mydisplay,mygc,myfont);
  } /* if */
  XFreeFontNames(fontlist);

  XCopyGC(mydisplay,mygc,-1,initgc);
  
  /* reset internal lists */

  worldlist = (struct win_node *) NULL;
  buttonflag = 0;
  buttonlist.buttons = (ButtonType *) NULL;
  buttonlist.NofButtons = 0;
}


/*****************************************************************************/

void ExitX()
{
  /* destroy remaining windows */

  while(worldlist) DestroyWindow(worldlist->win);

  /* free memory */

  if ( buttonlist.buttons ) free ( buttonlist.buttons );

  XFreeGC(mydisplay,mygc);
  XFreeGC(mydisplay,cleargc);
  
  XCloseDisplay(mydisplay);
}

/*****************************************************************************/

/* internal world- and win- handling */

struct world_node *make_world_node()
{
  return( (struct world_node *) calloc(1,sizeof(struct world_node)) );
}

struct win_node *make_win_node()
{
  return( (struct win_node *) calloc(1,sizeof(struct win_node)) );
}

struct win_node *find_win_node(Window window)
{
  struct win_node *winptr;
  int notfound=1;

  winptr = worldlist;

  while(winptr && notfound) {
    if( winptr->win == window ) notfound = 0;
    else winptr = winptr->nextwin;
  }
  return winptr;
}

struct world_node *find_world_node(World world)
{
  struct world_node *worldptr;
  int notfound=1;

  worldptr = find_win_node(world->win)->nextworld;

  while(worldptr && notfound) {
    if( worldptr->world == world ) notfound = 0;
    else worldptr = worldptr->nextworld;
  }
  return worldptr;
}


/*****************************************************************************/

Window CreateWindow(int width, int height, char *name)
{
  Window tmp;
  struct win_node *winptr;

  XSizeHints *sizehints;
  XWMHints   *wmhints;
  XClassHint *classhints;
  XTextProperty windowname, iconname;


  /* define window attributes */

  if(!(sizehints=XAllocSizeHints())){
    fprintf(stderr,"Xgraphics: failure allocating memory\n");
    exit(-1);
  }

  if(!(wmhints=XAllocWMHints())){
    fprintf(stderr,"Xgraphics: failure allocating memory\n");
    exit(-1);
  }

  if(!(classhints=XAllocClassHint())){
    fprintf(stderr,"Xgraphics: failure allocating memory\n");
    exit(-1);
  }

  if(XStringListToTextProperty(&name,1,&windowname) ==0) {
    fprintf(stderr,"Xgraphics: failure allocating structure\n");
    exit(-1);
  }

  if(XStringListToTextProperty(&name,1,&iconname) ==0) {
    fprintf(stderr,"Xgraphics: failure allocating structure\n");
    exit(-1);
  }

  wmhints->initial_state = NormalState;
  wmhints->input = True;
  wmhints->flags = StateHint | InputHint;

/*
  sizehints->base_width = width;
  sizehints->base_height = height;
  sizehints->flags = PBaseSize;
*/

  attributes.backing_store    = Always;
  attributes.background_pixel = white;

  /* create a window */

  tmp =  XCreateWindow(mydisplay,DefaultRootWindow(mydisplay),
             200,200,width,height,                      /* coords */
             17,CopyFromParent,            /* border width, depth */
             InputOutput,                                /* class */
             CopyFromParent,                       /* visual type */
             CWBackingStore|
	     CWBackPixel,                            /* valuemask */
             &attributes);                          /* attributes */
  
  XSelectInput(mydisplay,tmp,
	    OwnerGrabButtonMask |   
            ButtonPressMask | ButtonReleaseMask |         /* Mouse */
            EnterWindowMask |               /* Mouse enters Window */
	    StructureNotifyMask |              /* Geometry changes */
            KeyPressMask    |                          /* Keyboard */
            ExposureMask);                    /* Window - Exposure */

  XSetWMProperties(mydisplay,tmp,&windowname,&iconname,NULL,0,sizehints,
		   wmhints,classhints);


  /* update window/world - database */

  winptr    = worldlist;
  worldlist = make_win_node();

  worldlist->nextwin   = winptr;
  worldlist->prevwin   = (struct win_node *) NULL;
  worldlist->nextworld = (struct world_node *) NULL;
  worldlist->win       = tmp;

  if(winptr) winptr->prevwin = worldlist;

  return tmp;
}
  
/*****************************************************************************/

void ShowWindow(Window win)
{
  XMapRaised(mydisplay,win);
}

void HideWindow(Window win)
{
  XUnmapWindow(mydisplay,win);
  XSync(mydisplay,True);
}

/*****************************************************************************/

void DestroyWindow(Window win)
{
  struct win_node   *winptr;
  struct world_node *worldptr;

  /* find entry in database */
  winptr = find_win_node(win);
  worldptr = winptr->nextworld;

  /* destroy worlds within this window */
  while(winptr->nextworld) DestroyWorld(winptr->nextworld->world);

  /* delete database-entry */
  if(winptr->nextwin) winptr->nextwin->prevwin = winptr->prevwin;
  if(winptr->prevwin) winptr->prevwin->nextwin = winptr->nextwin;
  else worldlist = winptr->nextwin;

  /* destroy window */
  XDestroyWindow(mydisplay,win);
  free(winptr);
}

void DestroyWorld(World world) 
{
  struct world_node *worldptr;
  
  /* find database-entry */
  worldptr = find_world_node(world);

  /* free GC's */
  if(world->clipping) XFreeGC(mydisplay,world->gcw);
  if(world->gcp != mygc) XFreeGC(mydisplay,world->gcp);

  /* delete database-entry */
  if(worldptr->nextworld) worldptr->nextworld->prevworld = worldptr->prevworld;
  if(worldptr->prevworld) worldptr->prevworld->nextworld = worldptr->nextworld;
  else find_win_node(world->win)->nextworld = worldptr->nextworld;

  /* destroy world */
  free(world);
  free(worldptr);
}


/*****************************************************************************/

void ClearWindow(Window win)
{
  XClearWindow(mydisplay,win);
}

/*****************************************************************************/

World CreateWorld(Window window, int px, int py, int pwidth, int pheight,
		  double wx1, double wy1, double wx2, double wy2,
		  int scalable, int gravity)
{
 
  World  tmpworld;
  struct win_node   *winptr;
  struct world_node *worldptr;
  Window tmp;
  int    gx,gy;
  unsigned int gb,gd;
  XRectangle rect;

  /* create world (memory) */
  tmpworld = (World) calloc(1,sizeof(Worldstruct));

  /* if neccessary create window */
  if(window == 0) { tmpworld->win = CreateWindow(px+pwidth,py+pheight,(char*)"");
		    ShowWindow(tmpworld->win); }
  else            tmpworld->win = window;

  /* create Backing-storage Pixmap */
  if( !scalable ) {
    tmpworld->pixmap = XCreatePixmap(mydisplay,tmpworld->win,pwidth,pheight,
				    DefaultDepth(mydisplay,myscreen));
    XFillRectangle(mydisplay,tmpworld->pixmap,cleargc,0,0,pwidth,pheight);
  }
  else
    tmpworld->pixmap = 0;

  /* set coords */
  tmpworld->px = px;
  tmpworld->py = py;
  tmpworld->pwidth  = pwidth;
  tmpworld->pheight = pheight;
  
  tmpworld->bbx = px;
  tmpworld->bby = py;
  tmpworld->bbwidth = pwidth;
  tmpworld->bbheight = pheight;

  tmpworld->x1 = wx1;
  tmpworld->y1 = wy1;
  tmpworld->x2 = wx2;
  tmpworld->y2 = wy2;

  tmpworld->fx = (double)(pwidth-1)/(wx2-wx1);
  tmpworld->fy = (double)(pheight-1)/(wy2-wy1);

  tmpworld->aspect = (double)pheight/(double)pwidth;

  /* get coords of window */
  XGetGeometry(mydisplay,tmpworld->win,&tmp,&gx,&gy,
	       &(tmpworld->winwidth),&(tmpworld->winheight),&gb,&gd);

  /* set params */
  tmpworld->scalable = scalable;
  tmpworld->gravity  = gravity;
  tmpworld->clipping = CLIPPING;

  /* create CG for Pixmap */
  tmpworld->gcp = XCreateGC(mydisplay,DefaultRootWindow(mydisplay),0,0);
  XCopyGC(mydisplay,initgc,-1,tmpworld->gcp);

  /* create CG for world */
  if (CLIPPING) {
    tmpworld->gcw = XCreateGC(mydisplay,DefaultRootWindow(mydisplay),0,0);
    XCopyGC(mydisplay,initgc,-1,tmpworld->gcw);
    rect.x = px;
    rect.y = py;
    rect.width  = pwidth;
    rect.height = pheight;
    XSetClipRectangles(mydisplay,tmpworld->gcw,0,0,&rect,1,Unsorted);
  } else 
    tmpworld->gcw = tmpworld->gcp;

  /* set default color */
  tmpworld->color = black;
  tmpworld->function = GXcopy;

  /* create database-entry */
  winptr = find_win_node(tmpworld->win);
  if(winptr){
    worldptr = winptr->nextworld;
    winptr->nextworld = make_world_node();
    winptr->nextworld->world = tmpworld;
    winptr->nextworld->nextworld = worldptr;
    winptr->nextworld->prevworld = (struct world_node *) NULL;

    if(worldptr) worldptr->prevworld = winptr->nextworld;
  }
  else { fprintf(stderr,"Problems with alloc !\n"); exit(0); }

  return tmpworld;
}  

/*****************************************************************************/

void ClearWorld(World world)
{
  ClearArea(world->win,world->px,world->py,world->pwidth,world->pheight);

  if(world->pixmap) XFillRectangle(mydisplay,world->pixmap,cleargc,
				  0,0,
				  world->pwidth,world->pheight);
  XFlush(mydisplay);
}

/*****************************************************************************/

void ExposeWorld(World world)
{
  if(world->pixmap) {
    XCopyArea(mydisplay,world->pixmap,world->win,mygc,0,0,
	      world->pwidth,world->pheight,
	      world->px,world->py);
    XFlush(mydisplay);
  }
}

/*****************************************************************************/

void ResizeWorld(World world, int newwidth, int newheight)
{
  int delta_width, delta_height, bbx_a, bby_a;
  XRectangle rect;

  /* differences in size */
  delta_width  = (newwidth-world->winwidth);
  delta_height = (newheight-world->winheight);

  /* bounding-box */
  bbx_a = world->bbx;
  bby_a = world->bby;

  /* new coords for non-scalable worlds */
  if( !(world->scalable) )
    switch(world->gravity) {
    case NorthGravity:
      world->px += delta_width/2;
      break;
      
    case EastGravity:
      world->px += delta_width;
      world->py += delta_height/2;
      break;
      
    case SouthGravity:
      world->px += delta_width/2;
      world->py += delta_height;
      break;
      
    case WestGravity:
      world->py += delta_height/2;
      break;
      
    case NorthEastGravity:
      world->px += delta_width;
      break;
      
    case SouthEastGravity:
      world->px += delta_width;
      world->py += delta_height;
      break;
      
    case SouthWestGravity:
      world->py += delta_height;
      break;
      
    case NorthWestGravity:
    default:
      break;

  } else {

  /* scalable worlds */
    if( world->scalable & FIXED_WIDTH ) {
      switch(world->gravity) {
      case NorthWestGravity:
      case WestGravity:
      case SouthWestGravity:
	break;
      case NorthEastGravity:
      case EastGravity:
      case SouthEastGravity:
	world->bbx += delta_width;
	break;
      default:
	world->bbx += delta_width/2;
	break;
      }
    } else {
      world->bbx += (world->scalable & FLOAT_WEST)  ? 
	world->bbx*(delta_width/(double)(world->winwidth)) : 0;
      
      world->bbwidth = (world->scalable & FLOAT_EAST) ? 
	(bbx_a + world->bbwidth)*
	  (newwidth/(double)(world->winwidth)) - world->bbx:
	  (world->bbwidth + delta_width) + (bbx_a - world->bbx);
    }
    
    if( world->scalable & FIXED_HEIGHT ) {
      switch(world->gravity) {
      case NorthEastGravity:
      case NorthGravity:
      case NorthWestGravity:
	break;
      case SouthEastGravity:
      case SouthGravity:
      case SouthWestGravity:
	world->bby += delta_height;
	break;
      default:
	world->bby += delta_height/2;
	break;
      }
    } else {
      
      world->bby += (world->scalable & FLOAT_NORTH) ? 
	world->bby*(delta_height/(double)(world->winheight)) : 0;
      
      world->bbheight = (world->scalable & FLOAT_SOUTH) ? 
	(bby_a + world->bbheight)*
	  (newheight/(double)(world->winheight)) - world->bby:
	  (world->bbheight + delta_height) + (bby_a - world->bby);
    }
    
    if ( world->scalable & FREE_ASPECT ) {
      world->px = world->bbx; world->py = world->bby;
      world->pwidth = world->bbwidth; world->pheight = world->bbheight;
    } else {
      if ( world->bbwidth * world->aspect > world->bbheight ) {
	world->pheight = world->bbheight;
	world->pwidth = world->pheight/world->aspect;
	world->py = world->bby;
	world->px = world->bbx + (world->bbwidth - world->pwidth)/2;
      } else {
	world->pwidth = world->bbwidth;
	world->pheight = world->pwidth*world->aspect;
	world->px = world->bbx;
	world->py = world->bby + (world->bbheight - world->pheight)/2;
      }
    }
 
    world->fx = (double)(world->pwidth-1)/(world->x2 - world->x1);
    world->fy = (double)(world->pheight-1)/(world->y2 - world->y1);
  }
  world->winwidth  = newwidth;
  world->winheight = newheight;

  if ( world->pwidth  < 1 ) world->pwidth  = 1;
  if ( world->pheight < 1 ) world->pheight = 1;
  
  world->aspect = (double)world->pheight/(double)world->pwidth;
  if (world->clipping) {
    rect.x = world->px;
    rect.y = world->py;
    rect.width  = world->pwidth;
    rect.height = world->pheight;
    XSetClipRectangles(mydisplay,world->gcw,0,0,&rect,1,Unsorted);
  }
}

/*****************************************************************************/

void    RescaleWorld(World world, double nx1, double ny1, double nx2, double ny2)
{
  world->x1 = nx1;
  world->y1 = ny1;
  world->x2 = nx2;
  world->y2 = ny2;

  world->fx = (double)(world->pwidth-1)/(nx2-nx1);
  world->fy = (double)(world->pheight-1)/(ny2-ny1);
}

/*****************************************************************************/

void InitButtons(Window win, const char* buttonstring, 
		 int width)
{
  struct blist { char type; char text[20]; char key; } *list; 

  XSetWindowAttributes b_attributes;
  const char *ptr;
  char  *ptr2;
  int i, j, count = 1,bcount = 0, x, height, gx, gy;
  unsigned int gwidth, gheight, gb, gd;
  Window tmp;

  ptr = buttonstring;
  while(*ptr) if(*ptr++ == ',') (count)++;
  count /= 3;

  /* parse buttonstring */

  list = (struct blist *) calloc(count,sizeof(struct blist));

  ptr = buttonstring; i=0;
  while(*ptr && i<count) {
    list[i].type = *ptr++;
    while ( (*ptr++ != ',') && *ptr); 
    ptr2 = list[i].text;
    for(j=0; (*ptr!=',') && (*ptr) && j<30 ; *ptr2++ = *ptr++,j++ );
    if(*ptr++ !=',') { printf("Fehler 2 ! %d \n",i); exit(0); };
    if (list[i].type == 'b' ) list[i].key = *ptr++; else list[i].key = ' ';
    while (*ptr++ !=',' && i<count-1);   
    i++;
  }

  /* count buttons */
  for(i=0,bcount=0;i<count;++i) if (list[i].type == 'b') ++bcount;

  /* calculate geometry */
  height = (count + 1) * BUTTONHEIGHT;
  XGetGeometry(mydisplay, win, &tmp, &gx, &gy, &gwidth, &gheight, &gb, &gd );
  x = gwidth - width;

  /* remove existing buttons */
  if ( buttonlist.buttons ) {
    free( buttonlist.buttons );
    ClearWorld( buttonlist.world );
    DestroyWorld( buttonlist.world );
  }

  /* create button-world */
  buttonlist.buttons = (ButtonType *) calloc(bcount,sizeof(ButtonType));
  buttonlist.world = CreateWorld(win,x,0,width,height,0,0,
				 width-1,height-1,0,NorthEastGravity);
  buttonlist.NofButtons = bcount;

  b_attributes.win_gravity = NorthEastGravity;

  /* create buttons */
  for(i=0,j=0;i<count;++i) {
    if ( list[i].type == 'b' ) {
/*
      WFillRectangle(buttonlist.world,5,i*BUTTONHEIGHT+2,
		     width-10,(i+1)*BUTTONHEIGHT-4,0);
*/
      WDrawRectangle(buttonlist.world,5,i*BUTTONHEIGHT+2,
		     width-10,(i+1)*BUTTONHEIGHT-4,1);
      WDrawString(buttonlist.world,10,(i+1)*BUTTONHEIGHT-13,list[i].text,1);

      buttonlist.buttons[j].win = 
	XCreateWindow(mydisplay,win,x+5,i*BUTTONHEIGHT+2,
		      width-12,BUTTONHEIGHT-4,0,CopyFromParent,
		      InputOnly,CopyFromParent,
		      CWWinGravity,&b_attributes);

      XSelectInput(mydisplay,buttonlist.buttons[j].win,ButtonPressMask);
      ShowWindow(buttonlist.buttons[j].win);

      buttonlist.buttons[j].keycode = XKeysymToKeycode(mydisplay,list[i].key);

      /* workaround from 
       http://stackoverflow.com/questions/9838385/replace-of-xkeycodetokeysym
      */
      int keysyms_per_keycode_return ;
      KeySym *keysym = XGetKeyboardMapping(
                                           mydisplay,
                                           buttonlist.buttons[j].keycode,
                                           1,
                                           &keysyms_per_keycode_return) ; 
      if (keysym[0] == list[i].key)
         buttonlist.buttons[j++].state = 0;
      else
       buttonlist.buttons[j++].state = 1;
      XFree(keysym) ;


    } else {

      WDrawString(buttonlist.world,5,(i+1)*BUTTONHEIGHT-13,list[i].text,1);
    }
  }
  buttonflag = 1;
  free(list);
}

/*****************************************************************************/

int GetEvent(XEvent* event, int wait_flag)
{
  int i, ret;
  int newwidth, newheight;
  struct win_node   *winptr;
  struct world_node *worldptr;
  static int configure_flag=0;
  char buffer[2];

  if(wait_flag) 
  {
    /* wait for next event */
    XNextEvent(mydisplay,event);
    ret = 1;
  }
  else
  {
    /* check whether an event is queued */
    if(XEventsQueued(mydisplay,QueuedAfterFlush)) 
    {
      /* read event */
      XNextEvent(mydisplay,event);
      ret = 1;
    }
    else
    {
      /* if no event queued, return 0 */
      event->type = 0;
      ret = 0;
    }
  };
  
  /* serve standard-events */
  if(ret)
    {
      switch(event->type) {
	
      case EnterNotify:
	/* mouse entered window */
/*	XSetInputFocus(mydisplay,event->xcrossing.window,
		       RevertToParent,CurrentTime);
*/
	break;
	
      case MappingNotify:
	/* someone changed keyboard-layout */
	XRefreshKeyboardMapping(&(event->xmapping));
	break;
	
      case Expose:
	/* window gets visible again */
	if(configure_flag) {
	  XClearWindow(mydisplay,event->xexpose.window);
	  
	  winptr = find_win_node(event->xexpose.window);
	  worldptr = winptr->nextworld;
	  
	  while(worldptr) {
	    ExposeWorld(worldptr->world);
	    worldptr = worldptr->nextworld;
	  }
	} else {
	  event->type = 0;
	  ret = 0;	
	}
	configure_flag = 0;
	break;
	
      case ConfigureNotify:
	/* window geometry changed */
	newwidth  = event->xconfigure.width;
	newheight = event->xconfigure.height;
	
	winptr = find_win_node(event->xconfigure.window);
	worldptr = winptr->nextworld;
	
	while(worldptr) {
	  
	  ResizeWorld(worldptr->world,newwidth,newheight);
	  worldptr = worldptr->nextworld;
	}
	configure_flag = 1;
	break;
	
      case ButtonPress:
	/* someone pressed a mouse button */
	if (buttonflag) {
	  /* check whether it was in a 'button' and return key-event */
	  for(i=0; i<buttonlist.NofButtons; i++) {
	    if (event->xbutton.window == buttonlist.buttons[i].win) {
	      event->type = KeyPress;
	      event->xkey.keycode = buttonlist.buttons[i].keycode;
	      event->xkey.state = buttonlist.buttons[i].state;
	      
	    }
	  }
	}
	break;
	
      case KeyPress:
	/* someone pressed a key */
	if (XLookupString(&(event->xkey),buffer,2,0,0) != 1) {
	  event->type = 0;
	  ret = 0;
	}
	break;
	
      default:
	break;
      }
    };
  return ret;
}

/*****************************************************************************/

char ExtractChar(XEvent event)
{
  char buffer[2];

  if(event.type != KeyPress) return 0;
  if(XLookupString(&event.xkey,buffer,2,0,0)==1) return buffer[0];
  else return 0;
}

/*****************************************************************************/

int WGetMousePos(World world, XEvent event, double *x, double *y)
{
  int ret = 0;

  if(event.type != ButtonPress) return 0;

  /* get coords and convert to world-coords */
  *x = (event.xbutton.x - world->px)/world->fx + world->x1;
  *y = (event.xbutton.y - world->py)/world->fy + world->y1;

  /* in world ? */
  if ( (*x >= world->x1) && (*x <= world->x2) && 
       (*y >= world->y1) && (*y <= world->y2) ) ret = event.xbutton.button;

  return ret;
}
  
/*****************************************************************************/


int GetNumber( Window win, int x, int y, double *value )
{
  char   str[30],  zeichen = ' ';
  int    i=1;
  XEvent myevent;
  
  str[0] = ' ';str[1] = '\0';
  
  while ( zeichen != '\15' )           /* repeat until RETURN pressed */
    {  
      GetEvent(&myevent,1);
      
      switch (myevent.type){
	
      case KeyPress: 
	zeichen = ExtractChar(myevent);
	switch(zeichen){
	  
          /**********   BACK SPACE  ***************************************/
	  
	case '\10':
	  {     
	    if(i>0)
	      {
		
		DrawString (win, x,y,str,1);
		
		str[i+1] = '\0'; str[i] = ' '; str[--i] = ' '; 
		DrawString (win, x,y,str,1);
		break;
	      }
	    else XBell(mydisplay,0);
	  }
	  
          /**********   RETURN   ******************************************/
	  
	case '\15':
	  {     str[i] = '\0';
		break; }
	  
	  
          /**********   MINUS SIGN  ***************************************/
	  
	case '-'  :
	  {     if (i>0) i--; }

	  
          /**********   NUMBER   ******************************************/
	  
	case '0'  :
	case '1'  :
	case '2'  :
	case '3'  :
	case '4'  :
	case '5'  :
	case '6'  :
	case '7'  :
	case '8'  :
	case '9'  :
	case '.'  :
	  
	  
	  {        str[i++] = zeichen;
		   str[i] = ' ';
		   str[i+1] = ' ';
		   str[i+2] = '\0';
		   DrawString (win, x,y,str,1);
		   break;}
	} /* switch zeichen */
	break;
	
      default: break;
      } /* switch */
    } /* while */
  
  
  /**************   Conversion to double *******************************/
  
  if ( !( str[0] == '\0' || str[1] == '\0') ) {
    *value = atof(str);
    return 1;
  } else return 0;
}



/*****************************************************************************/

int WGetNumber( World world, double x, double y, double *value )
{
  char   str[30],  zeichen = ' ';
  int    i=1;
  XEvent myevent;
  
  str[0] = ' ';str[1] = '\0';
  
  while ( zeichen != '\15' )           /* repeat until RETURN pressed */
    {  
      GetEvent(&myevent,1);
      
      switch (myevent.type){
	
      case KeyPress: 
	zeichen = ExtractChar(myevent);
	switch(zeichen){
	  
          /**********   BACK SPACE  ***************************************/
	  
	case '\10':
	  {     if(i>0)
		  {
		    
		    WDrawString (world, x,y,str,1);
		    
		    str[i+1] = '\0'; str[i] = ' '; str[--i] = ' '; 
		    WDrawString (world, x,y,str,1);
		    break;
		  }
	  else XBell(mydisplay,0);
	      }
	  
          /**********   RETURN   ******************************************/
	  
	case '\15':
	  {     str[i] = '\0';
		break; }
	  
	  
          /**********   MINUS SIGN  ***************************************/
	  
	case '-'  :
	  {     if (i>0) i--; }
	  
	  
          /**********   NUMBER   ******************************************/
	  
	case '0'  :
	case '1'  :
	case '2'  :
	case '3'  :
	case '4'  :
	case '5'  :
	case '6'  :
	case '7'  :
	case '8'  :
	case '9'  :
	case '.'  :
	  
	  
	  {        str[i++] = zeichen;
		   str[i] = ' ';
		   str[i+1] = ' ';
		   str[i+2] = '\0';
		   WDrawString (world, x,y,str,1);
		   break;}
	} /* switch zeichen */
	break;
	
      default: break;
      } /* switch */
    } /* while */
  
  
  /**************   Conversion to double *******************************/
  
  if ( !( str[0] == '\0' || str[1] == '\0') ) {
    *value = atof(str);
    return 1;
  }
  else return 0;
}

/*****************************************************************************/

int GetString( Window win, int x, int y, int length, char *str )
{
  char   zeichen = ' ';
  int    i=0;
  XEvent myevent;
  
  str[0] = ' ';str[1] = '\0';
  
  while ( zeichen != '\15' )           /* repeat until RETURN pressed */
    {  
      GetEvent(&myevent,1);
      
      switch (myevent.type){
	
      case KeyPress: 
	zeichen = ExtractChar(myevent);
	switch(zeichen){
	  
          /**********   BACK SPACE  ***************************************/
	  
	case '\10':
	  {     
	    if(i>0)
	      {
		
		DrawString (win, x,y,str,1);
		
		XBell(mydisplay,0);

		str[i+1] = '\0'; str[i] = ' '; str[--i] = ' '; 
		DrawString (win, x,y,str,1);
		break;
	      }
	    else XBell(mydisplay,0);
	  }
	  
          /**********   RETURN   ******************************************/
	  
	case '\15':
	  {     str[i] = '\0';
		break; }
	  
	  
          /**********  CHARACTER   ****************************************/
	  
	default  :
	  
	  
	  {        str[i++] = zeichen;
		   str[i] = ' ';
		   str[i+1] = ' ';
		   str[i+2] = '\0';
		   DrawString (win, x,y,str,1);
		   break;}
	} /* switch zeichen */
	break;
	
      } /* switch */
    } /* while */
  
  
  /******************************************************************/
  
  if ( !( str[0] == '\0' || str[1] == '\0') ) {
    str[i] = '\0';
    return 1;
  } else return 0;
}



/*****************************************************************************/

int WGetString( World world, double x, double y, int length, char *string )
{
  char   str[30],  zeichen = ' ';
  int    i=0;
  XEvent myevent;
  
  str[0] = ' ';str[1] = '\0';
  
  while ( zeichen != '\15' )           /* repeat until RETURN pressed */
    {  
      GetEvent(&myevent,1);
      
      switch (myevent.type){
	
      case KeyPress: 
	zeichen = ExtractChar(myevent);
	switch(zeichen){
	  
          /**********   BACK SPACE  ***************************************/
	  
	case '\10':
	  {     if(i>0)
		  {
		    
		    WDrawString (world, x,y,str,1);
		    
		    str[i+1] = '\0'; str[i] = ' '; str[--i] = ' '; 
		    WDrawString (world, x,y,str,1);
		    break;
		  }
	  else XBell(mydisplay,0);
	      }
	  
          /**********   RETURN   ******************************************/
	  
	case '\15':
	  {     str[i] = '\0';
		break; }
	  
	  
          /**********   CHARACTER   ***************************************/
	  
	default  :
	  
	  
	  {        str[i++] = zeichen;
		   str[i] = ' ';
		   str[i+1] = ' ';
		   str[i+2] = '\0';
		   WDrawString (world, x,y,str,1);
		   break;}
	} /* switch zeichen */
	break;
	
      } /* switch */
    } /* while */
  
  
  /***************************************************************/
  
  if ( !( str[0] == '\0' || str[1] == '\0') ) {
    str[i] = '\0';
    strcpy(string,str);
    return 1;
  }
  else return 0;
}



/*****************************************************************************/

void WSetColor(World world, unsigned long color_func)
{
  unsigned long color;
  unsigned function;

  /* extract color and drawing-function */
  
  function  = (color_func&4096)?(color_func/752)%16:3;
  color     = (function==GXxor)?                  /* colors should appear */
    mycolors[(color_func%752)%NofColors]^white:   /* normal on white      */ 
      mycolors[(color_func%752)%NofColors];       /* background           */
  
  if(color != world->color) {
    XSetForeground(mydisplay,world->gcw,color);
    if(world->pixmap) XSetForeground(mydisplay,world->gcp,color);
    world->color = color;
  }

  if(function != world->function) {
    XSetFunction(mydisplay,world->gcw,function);
    if(world->pixmap) XSetFunction(mydisplay,world->gcp,function);
    world->function = function;
  }
}  


void SetColor(unsigned long color_func)
{
  unsigned long color;
  unsigned function;

  function  = (color_func&4096)?(color_func/752)%16:3;
  color     = (function==GXxor)?
    mycolors[(color_func%752)%NofColors]^white:
      mycolors[(color_func%752)%NofColors];
  
  if(color != global_color) {
    XSetForeground(mydisplay,mygc,color);
    global_color = color;
  }

  if(function != global_function) {
    XSetFunction(mydisplay,mygc,function);
    global_function = function;
  }
  
}  


/*****************************************************************************/

void ClearArea(Window win, int x, int y, int width, int height)
{
  XClearArea(mydisplay,win,x,y,width,height,False);
}

/*****************************************************************************/

void DrawPoint(Window win, int x, int y, int c)
{
  SetColor(c);

  XDrawPoint(mydisplay,win,mygc,x,y);
}

/*****************************************************************************/

void DrawPoints(Window win, XPoint *points, int NofPoints, int c)
{
  SetColor(c);

  XDrawPoints(mydisplay, win, mygc, points, NofPoints, CoordModeOrigin);
}

/*****************************************************************************/

void DrawLine(Window win, int x1,int y1, int x2, int y2, int c)
{
  SetColor(c);

  XDrawLine(mydisplay,win,mygc,x1,y1,x2,y2);
}

/*****************************************************************************/

void DrawLines(Window win, XPoint *points, int NofPoints, int c)
{
  SetColor(c);

  XDrawLines(mydisplay, win, mygc, points, NofPoints, CoordModeOrigin);
}


/*****************************************************************************/

void DrawCircle(Window win, int x, int y, int r, int c)
{
  int rr;
  
  rr = abs(r);
  SetColor(c);

  XDrawArc(mydisplay,win,mygc,x-rr,y-rr,2*rr,2*rr,0,23040);
}

void FillCircle(Window win, int x, int y, int r, int c)
{
  int rr;
  
  rr = abs(r);
  SetColor(c);

  XFillArc(mydisplay,win,mygc,x-rr,y-rr,2*rr,2*rr,0,23040);
}


/*****************************************************************************/

void DrawString(Window win, int x, int y, const char* text, int c)
{
  SetColor(c);

  XDrawImageString(mydisplay,win,mygc,x,y,text,strlen(text));
  XFlush(mydisplay);
}

/*****************************************************************************/

void DrawRectangle(Window win, int x1, int y1, int x2, int y2, int c)
{
  int temp;
  SetColor(c);

  if (x1 > x2) { temp=x1; x1=x2; x2=temp; };
  if (y1 > y2) { temp=y1; y1=y2; y2=temp; };

  XDrawRectangle(mydisplay, win, mygc, x1, y1, x2-x1, y2-y1);
  XFlush(mydisplay);
}

void FillRectangle(Window win, int x1, int y1, int x2, int y2, int c)
{
  int temp;
  SetColor(c);

  if (x1 > x2) { temp=x1; x1=x2; x2=temp; };
  if (y1 > y2) { temp=y1; y1=y2; y2=temp; };

  XFillRectangle(mydisplay, win, mygc, x1, y1, x2-x1, y2-y1);
  XFlush(mydisplay);
}

/*****************************************************************************/

void DrawPoly(Window win, XPoint *points, int NofPoints, int c)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints+1,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XLines !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = points[i].x;
    xpoints[i].y = points[i].y;
  }

  if ((xpoints[0].x != xpoints[NofPoints-1].x)
    ||(xpoints[0].y != xpoints[NofPoints-1].y)) {
      xpoints[NofPoints].x = xpoints[0].x;
      xpoints[NofPoints].y = xpoints[0].y;
      NofPoints ++; 
    }
  

  SetColor(c);

  XDrawLines(mydisplay,win,mygc,
	      xpoints,NofPoints,CoordModeOrigin);

  free(xpoints);

  XFlush(mydisplay);
}

/*****************************************************************************/

void FillPoly(Window win, XPoint *points, int NofPoints, int c, int cfill)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints+1,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XLines !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = points[i].x;
    xpoints[i].y = points[i].y;
  }

  if ((xpoints[0].x != xpoints[NofPoints-1].x)
    ||(xpoints[0].y != xpoints[NofPoints-1].y)) {
      xpoints[NofPoints].x = xpoints[0].x;
      xpoints[NofPoints].y = xpoints[0].y;
      NofPoints ++; 
    }
  
  SetColor(cfill);

  XFillPolygon(mydisplay, win, mygc, points, NofPoints, Complex,
	       CoordModeOrigin);


  SetColor(c);

  XDrawLines(mydisplay,win,mygc,
	      xpoints,NofPoints,CoordModeOrigin);

  free(xpoints);

  XFlush(mydisplay);
}

/*****************************************************************************/


void WDrawPoint(World world, double x, double y, int c)
{
  int px, py;

  px = (x-world->x1)*world->fx;
  py = (y-world->y1)*world->fy;

  WSetColor(world,c);

  XDrawPoint(mydisplay,world->win,world->gcw,world->px+px,world->py+py);
  if(world->pixmap) XDrawPoint(mydisplay,world->pixmap,world->gcp,px,py);
  XFlush(mydisplay);
}

/*****************************************************************************/

void WDrawPoints(World world, WPoint *points, int NofPoints, int c)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XPoints !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = world->px + (points[i].x-world->x1)*world->fx;
    xpoints[i].y = world->py + (points[i].y-world->y1)*world->fy;
  }

  WSetColor(world,c);

  XDrawPoints(mydisplay,world->win,world->gcw,
	      xpoints,NofPoints,CoordModeOrigin);

  if(world->pixmap){
    for(i=0; i<NofPoints; ++i) {
      xpoints[i].x -= world->px;
      xpoints[i].y -= world->py;
    }

    XDrawPoints(mydisplay,world->pixmap,world->gcp,
		xpoints,NofPoints,CoordModeOrigin);
  }
  free(xpoints);
}

/*****************************************************************************/

void WDrawLine(World world, double x1, double y1, double x2, double y2, int c)
{
  int px1, px2, py1, py2;

  px1 = (x1-world->x1)*world->fx;
  py1 = (y1-world->y1)*world->fy;
  px2 = (x2-world->x1)*world->fx;
  py2 = (y2-world->y1)*world->fy;

  WSetColor(world,c);

  XDrawLine(mydisplay,world->win,world->gcw,
	    world->px+px1,world->py+py1,world->px+px2,world->py+py2);
  
  if(world->pixmap) XDrawLine(mydisplay,world->pixmap,world->gcp,
			      px1,py1,px2,py2);
  
  XFlush(mydisplay);
}

/*****************************************************************************/

void WDrawLines(World world, WPoint *points, int NofPoints, int c)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XLines !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = world->px + (points[i].x-world->x1)*world->fx;
    xpoints[i].y = world->py + (points[i].y-world->y1)*world->fy;
  }

  WSetColor(world,c);

  XDrawLines(mydisplay,world->win,world->gcw,
	      xpoints,NofPoints,CoordModeOrigin);

  if(world->pixmap){
    for(i=0; i<NofPoints; ++i) {
      xpoints[i].x -= world->px;
      xpoints[i].y -= world->py;
    }

    XDrawLines(mydisplay,world->pixmap,world->gcp,
		xpoints,NofPoints,CoordModeOrigin);
  }
  free(xpoints);

  XFlush(mydisplay);
}

/*****************************************************************************/

void WDrawCircle(World world, double x, double y, double r, int c)
{
  int px, py, prx, pry;

  px = (x-world->x1)*world->fx;
  py = (y-world->y1)*world->fy;
  prx = abs((int) (r*world->fx));
  pry = abs((int) (r*world->fy));

  if(prx==0) prx=1;
  if(pry==0) pry=1;

  WSetColor(world,c);

  XDrawArc(mydisplay,world->win,world->gcw,world->px+px-prx,
	   world->py+py-pry,2*prx,2*pry,0,23040);

  if(world->pixmap) XDrawArc(mydisplay,world->pixmap,world->gcp,px-prx,
			    py-pry,2*prx,2*pry,0,23040);
}

void WFillCircle(World world, double x, double y, double r, int c)
{
  int px, py, prx, pry;

  px = (x-world->x1)*world->fx;
  py = (y-world->y1)*world->fy;
  prx = abs((int) (r*world->fx));
  pry = abs((int) (r*world->fy));

  if(prx==0) prx=1;
  if(pry==0) pry=1;

  WSetColor(world,c);

  XFillArc(mydisplay,world->win,world->gcw,world->px+px-prx,
	   world->py+py-pry,2*prx,2*pry,0,23040);

  if(world->pixmap) XFillArc(mydisplay,world->pixmap,world->gcp,px-prx,
			    py-pry,2*prx,2*pry,0,23040);
}


/*****************************************************************************/

void WDrawString(World world, double x, double y, const char* text, int c)
{
  int px,py;

  px = (x-world->x1)*world->fx;
  py = (y-world->y1)*world->fy;
  
  WSetColor(world,c);

  XDrawImageString(mydisplay,world->win,world->gcw,
		   world->px+px,world->py+py,text,strlen(text));

  if(world->pixmap)   XDrawImageString(mydisplay,world->pixmap,world->gcp,
				      px,py,text,strlen(text));
  XFlush(mydisplay);
}

/*****************************************************************************/

void WDrawRectangle(World world, double x1, double y1, double x2, double y2, int c)
{
  int px1,px2,py1,py2, temp;

  px1 = (x1-world->x1)*world->fx;
  py1 = (y1-world->y1)*world->fy;
  px2 = (x2-world->x1)*world->fx;
  py2 = (y2-world->y1)*world->fy;

  if (px1>px2) { temp=px1; px1=px2; px2=temp; };
  if (py1>py2) { temp=py1; py1=py2; py2=temp; };

  WSetColor(world,c);

  XDrawRectangle(mydisplay, world->win, world->gcw, 
		 world->px+px1, world->py+py1, px2-px1, py2-py1);
  if(world->pixmap) XDrawRectangle(mydisplay, world->pixmap, world->gcp, 
				  px1, py1, px2-px1, py2-py1);
  XFlush(mydisplay);
}


void WFillRectangle(World world, double x1, double y1, double x2, double y2, int c)
{
  int px1,px2,py1,py2, temp;

  px1 = (x1-world->x1)*world->fx;
  py1 = (y1-world->y1)*world->fy;
  px2 = (x2-world->x1)*world->fx;
  py2 = (y2-world->y1)*world->fy;

  if (px1>px2) { temp=px1; px1=px2; px2=temp; };
  if (py1>py2) { temp=py1; py1=py2; py2=temp; };

  WSetColor(world,c);

  XFillRectangle(mydisplay, world->win, world->gcw, 
		 world->px+px1, world->py+py1, px2-px1, py2-py1);
  if(world->pixmap) XFillRectangle(mydisplay, world->pixmap, world->gcp, 
				  px1, py1, px2-px1, py2-py1);
  XFlush(mydisplay);
}

/*****************************************************************************/

void WDrawPoly(World world, WPoint *points, int NofPoints, int c)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints+1,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XLines !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = world->px + (points[i].x-world->x1)*world->fx;
    xpoints[i].y = world->py + (points[i].y-world->y1)*world->fy;
  }

  if ((xpoints[0].x != xpoints[NofPoints-1].x)
    ||(xpoints[0].y != xpoints[NofPoints-1].y)) {
      xpoints[NofPoints].x = xpoints[0].x;
      xpoints[NofPoints].y = xpoints[0].y;
      NofPoints ++; 
    }
  

  WSetColor(world,c);

  XDrawLines(mydisplay,world->win,world->gcw,
	      xpoints,NofPoints,CoordModeOrigin);

  if(world->pixmap){
    for(i=0; i<NofPoints; ++i) {
      xpoints[i].x -= world->px;
      xpoints[i].y -= world->py;
    }

    XDrawLines(mydisplay,world->pixmap,world->gcp,
		xpoints,NofPoints,CoordModeOrigin);
  }
  free(xpoints);

  XFlush(mydisplay);
}

/*****************************************************************************/

void WFillPoly(World world, WPoint *points, int NofPoints, int c, int cfill)
{
  int i;
  XPoint *xpoints;

  if( !(xpoints = (XPoint*) calloc(NofPoints+1,sizeof(XPoint)) ) ) {
    fprintf(stderr,"Calloc error XLines !!!\n");
    exit(-1);
  }


  for(i=0; i<NofPoints; ++i) {
    xpoints[i].x = world->px + (points[i].x-world->x1)*world->fx;
    xpoints[i].y = world->py + (points[i].y-world->y1)*world->fy;
  }

  WSetColor(world,cfill);

  XFillPolygon(mydisplay, world->win, world->gcw, xpoints, NofPoints,
	       Complex, CoordModeOrigin);

  if ((xpoints[0].x != xpoints[NofPoints-1].x)
    ||(xpoints[0].y != xpoints[NofPoints-1].y)) {
      xpoints[NofPoints].x = xpoints[0].x;
      xpoints[NofPoints].y = xpoints[0].y;
      NofPoints ++; 
    }
  

  WSetColor(world,c);

  XDrawLines(mydisplay,world->win,world->gcw,
	      xpoints,NofPoints,CoordModeOrigin);

  if(world->pixmap){
    for(i=0; i<NofPoints; ++i) {
      xpoints[i].x -= world->px;
      xpoints[i].y -= world->py;
    }
    
    WSetColor(world,cfill);

    XFillPolygon(mydisplay, world->pixmap, world->gcp, xpoints, NofPoints,
		 Complex, CoordModeOrigin);

    WSetColor(world,c);

    XDrawLines(mydisplay,world->pixmap,world->gcp,
		xpoints,NofPoints,CoordModeOrigin);
  }
  free(xpoints);

  XFlush(mydisplay);
}

/*****************************************************************************/

  





