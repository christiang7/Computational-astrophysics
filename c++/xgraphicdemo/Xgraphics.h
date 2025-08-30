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



#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifndef _XGRAPHICS_

#define _XGRAPHICS_


#define BUTTONHEIGHT 35
#define MAX_COLORS   16
#define CLIPPING      1
#define XOR          4096+256*GXxor

/* Constants for scalable */

#define SCALABLE       1
#define FREE_ASPECT    2
#define FLOAT_NORTH    4
#define FLOAT_SOUTH    8
#define FLOAT_EAST    16
#define FLOAT_WEST    32
#define FIXED_WIDTH   64
#define FIXED_HEIGHT 128


/* Declarations of used datatypes */

typedef struct world_type {
  Window   win;
  Pixmap   pixmap;
  GC       gcw, gcp;
  int      px, py, pwidth, pheight, 
           bbx, bby;
  unsigned int bbwidth, bbheight,
           winwidth, winheight;
  double   x1, y1, x2, y2;
  double   fx, fy, aspect;
  int      scalable;
  int      clipping;
  int      gravity;
  unsigned long color;
  int      function;
} Worldstruct, *World;


typedef struct w_point {
  double x, y;
} WPoint;


typedef struct button_type {
  Window  win;
  int     keycode;
  int     state;
} ButtonType;

typedef struct button_list {
  ButtonType *buttons;
  World      world;
  int        NofButtons;
} ButtonList;

/* Declaration of variables */

extern Display         *mydisplay;
extern int             myscreen;
extern Window          mywindow;
extern World           myworld;
extern unsigned long   black,white,global_color;
extern int             global_function;
extern GC              mygc,cleargc,initgc;
extern XEvent          myevent;
extern XSetWindowAttributes attributes;
extern int             buttonflag;
extern ButtonList      buttonlist;
extern unsigned long   mycolors[MAX_COLORS];
extern int             NofColors;
extern Colormap        mycmap;

static char *colorlist[] = {
  (char*)"white",
  (char*)"black",
  (char*)"red",
  (char*)"green",
  (char*)"blue",
  (char*)"yellow",
  (char*)"orange",
  (char*)"brown",
  (char*)"cyan",
  (char*)"gray",
  (char*)"tomato1",
  (char*)"limegreen",
  (char*)"lightblue",
  (char*)"yellow4",
  (char*)"orangered",
  (char*)"darkgreen",
  (char*)"blueviolet"
};

static char *graylist[] = {
  (char*)"white",
  (char*)"black",
  (char*)"gray90",
  (char*)"gray85",
  (char*)"gray80",
  (char*)"gray75",
  (char*)"gray70",
  (char*)"gray65",
  (char*)"gray60",
  (char*)"gray55",
  (char*)"gray50",
  (char*)"gray45",
  (char*)"gray40",
  (char*)"gray35",
  (char*)"gray30",
  (char*)"gray25",
  (char*)"gray20"
};


typedef struct world_node {
  World  world;
  struct world_node *prevworld,
                    *nextworld;
} world_node_t;

extern world_node_t*  make_world_node();
extern world_node_t*  find_world_node( World world );


typedef struct win_node {
  Window win;
  struct world_node *nextworld;
  struct win_node   *prevwin,
                    *nextwin;
} win_node_t;


extern win_node_t*  worldlist;
extern win_node_t*  make_win_node();
extern win_node_t*  find_win_node( Window window );




void WSetColor(World world, unsigned long color);
void SetColor(unsigned long color);

/*****************************************************************************/

/* Initialisation of X-parameters */

void    InitX();

/* 
   InitX() provides the connection to the X-server and sets defaults for
   the appearance of the windows.
*/


void    ExitX();

/* 
   ExitX() closes all remaining windows, frees the allocated memory and
   in the end shuts down the connection to the X-server.
*/


/*****************************************************************************/

/* Window-handling */

Window  CreateWindow(
		     int width, int height,         /* window size           */
		     char *name                     /* window title          */
		     );
/* 
   Creates a window with th given defaults.
*/


void    ShowWindow(Window win);

/* 
   Displays the window.
*/


void    HideWindow(Window win);

/* 
   Removes the window from the screen.
*/


void    DestroyWindow(Window win);

/*
   Closes the window.
*/


void    ClearWindow(Window win);

/*
   Clears the window.
*/

/*****************************************************************************/

/* World handling */



World   CreateWorld(
		    Window win, 
		    int px, int py, 
		    int pwidth, int pheight,
		    double wx1, double wy1, double wx2, double wy2,
       		    int scalable, 
		    int gravity
		    );

/*
   Creates a world in the window win with the window-coordinates (px,py)
   and the size (pwidth, pheight). Defines a new coordinatesystem with
   the upper left corner (wx1,wy1) and the lower right corner (wx2,wy2).
   The resizing behavior is defined by scalable and gravity.
*/


void    ClearWorld(World world);

/*
   Clears a World. 
*/

void    DestroyWorld(World world);

/*
   Destroys a world and frees allocated memory.
*/

void    ExposeWorld(World world);

/*
   Redraws non-scalable worlds. Used internal to handle Expose-events.
*/


void    ResizeWorld(World world, int newwidth, int newheight);

/*
   Changes the size of a scalable world after the window is resized.
*/


void    RescaleWorld(World world, double nx1, double ny1, double nx2, double ny2);

/*
   Changes the world-coordinate system.
*/

/*****************************************************************************/
/* Event handling */

void    InitButtons(Window win, const char* buttonstring, 
		    int width);

/*
   Creates a row of buttons at the right side of te window.
   The syntax of buttonstring is:
     (char*)"[t/b],[text],[key], [t/b],[text],[key],  ... "
   Entries with 't' create a textline, entries with 'b' create a button,
   which when clicked with the mouse, returns a KeyPress event for the
   key 'key'. This can be handled as a normal KeyPress event.
*/

int     GetEvent(XEvent* event, int wait_flag);

/*
   GetEvent reads an event from the event queue of the program.
   If wait_flag > 0, the function waits until the next event ( to
   conserv computing time ), otherwise the function return 0 and a 
   event type 0 if no event is in the queue.
*/


char    ExtractChar(XEvent event);

/*
   ExtractChar() returns the character code of the key pressed from an
   KeyEvent.
*/


int GetNumber(Window win, int x, int y, double *value);

/*
   GetNumber allows to enter a number in the window with editing the 
   line by using BACKSPACE. The input appears in the window at (x,y).
*/

int WGetNumber(World world, double x, double y, double *value);

/*
   Like GetNumber, but with world-coordinates.
*/


int GetString(Window win, int x, int y, int length, char *string);

/*
   GetNumber allows to enter a string in the window with editing the 
   line by using BACKSPACE. The input appears in the window at (x,y).
*/

int WGetString(World world, double x, double y, int length, char *string);

/*
   Like GetString, but with world-coordinates.
*/




int   WGetMousePos(World world, XEvent event, double *x, double *y);

/*
   Returns the position of a mouse click in coordinates of the world
   given as argument.
*/

/* Drawingroutines for Windows */

void    ClearArea(Window win, int x, int y, int width, int height); 

void    SetColor(unsigned long color);
void    SetFunction(int func);

void    DrawPoint(Window win, int x, int y, int c);
void    DrawPoints(Window win, XPoint *points, int NofPoints, int c);
void    DrawLine(Window win, int x1, int y1, int x2, int y2, int c);
void    DrawLines(Window win, XPoint *points, int NofPoints, int c);
void    DrawCircle(Window win, int x, int y, int r, int c);
void    FillCircle(Window win, int x, int y, int r, int c);
void    DrawString(Window win, int x, int y, const char* text, int c);
void    DrawRectangle(Window win, int x1, int y1, int x2, int y2, int c);
void    FillRectangle(Window win, int x1, int y1, int x2, int y2, int c);
void    DrawPoly(Window win, XPoint *points, int NofPoints, int c);
void    FillPoly(Window win, XPoint *points, int NofPoints, int c, int cfill);


/* Drawingroutines for worlds */

void    WSetColor(World world, unsigned long color);
void    WSetFunction(World world, int func);

void    WDrawPoint(World world, double x, double y, int c);
void    WDrawPoints(World world, WPoint *points, int NofPoints, int c);
void    WDrawLine(World world, double x1, double y1, double x2, double y2, int c);
void    WDrawLines(World world, WPoint *points, int NofPoints, int c);
void    WDrawCircle(World world, double x, double y, double r, int c);
void    WFillCircle(World world, double x, double y, double r, int c);
void    WDrawString(World world, double x, double y, const char* text, int c);
void    WDrawRectangle(World world, 
		       double x1, double y1, double x2, double y2, int c);
void    WFillRectangle(World world, 
		       double x1, double y1, double x2, double y2, int c);
void    WDrawPoly(World world, WPoint *points, int NofPoints, int c);
void    WFillPoly(World world, 
		       WPoint *points, int NofPoints, int c, int cfill);
		       
#endif










