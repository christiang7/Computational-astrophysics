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
#define MAX_COLORS   752
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

static char *colorlist[MAX_COLORS] = {
(char*) "white" ,
(char*) "black" ,
(char*) "snow" ,
(char*) "ghost" ,
(char*) "GhostWhite" ,
(char*) "WhiteSmoke" ,
(char*) "gainsboro" ,
(char*) "floral" ,
(char*) "FloralWhite" ,
(char*) "old" ,
(char*) "OldLace" ,
(char*) "linen" ,
(char*) "antique" ,
(char*) "AntiqueWhite" ,
(char*) "papaya" ,
(char*) "PapayaWhip" ,
(char*) "blanched" ,
(char*) "BlanchedAlmond" ,
(char*) "bisque" ,
(char*) "peach" ,
(char*) "PeachPuff" ,
(char*) "navajo" ,
(char*) "NavajoWhite" ,
(char*) "moccasin" ,
(char*) "cornsilk" ,
(char*) "ivory" ,
(char*) "lemon" ,
(char*) "LemonChiffon" ,
(char*) "seashell" ,
(char*) "honeydew" ,
(char*) "mint" ,
(char*) "MintCream" ,
(char*) "azure" ,
(char*) "alice" ,
(char*) "AliceBlue" ,
(char*) "lavender" ,
(char*) "lavender" ,
(char*) "LavenderBlush" ,
(char*) "misty" ,
(char*) "MistyRose" ,
(char*) "white" ,
(char*) "dark" ,
(char*) "DarkSlateGray" ,
(char*) "dark" ,
(char*) "DarkSlateGrey" ,
(char*) "dim" ,
(char*) "DimGray" ,
(char*) "dim" ,
(char*) "DimGrey" ,
(char*) "slate" ,
(char*) "SlateGray" ,
(char*) "slate" ,
(char*) "SlateGrey" ,
(char*) "light" ,
(char*) "LightSlateGray" ,
(char*) "light" ,
(char*) "LightSlateGrey" ,
(char*) "gray" ,
(char*) "grey" ,
(char*) "light" ,
(char*) "LightGrey" ,
(char*) "light" ,
(char*) "LightGray" ,
(char*) "midnight" ,
(char*) "MidnightBlue" ,
(char*) "navy" ,
(char*) "navy" ,
(char*) "NavyBlue" ,
(char*) "cornflower" ,
(char*) "CornflowerBlue" ,
(char*) "dark" ,
(char*) "DarkSlateBlue" ,
(char*) "slate" ,
(char*) "SlateBlue" ,
(char*) "medium" ,
(char*) "MediumSlateBlue" ,
(char*) "light" ,
(char*) "LightSlateBlue" ,
(char*) "medium" ,
(char*) "MediumBlue" ,
(char*) "royal" ,
(char*) "RoyalBlue" ,
(char*) "blue" ,
(char*) "dodger" ,
(char*) "DodgerBlue" ,
(char*) "deep" ,
(char*) "DeepSkyBlue" ,
(char*) "sky" ,
(char*) "SkyBlue" ,
(char*) "light" ,
(char*) "LightSkyBlue" ,
(char*) "steel" ,
(char*) "SteelBlue" ,
(char*) "light" ,
(char*) "LightSteelBlue" ,
(char*) "light" ,
(char*) "LightBlue" ,
(char*) "powder" ,
(char*) "PowderBlue" ,
(char*) "pale" ,
(char*) "PaleTurquoise" ,
(char*) "dark" ,
(char*) "DarkTurquoise" ,
(char*) "medium" ,
(char*) "MediumTurquoise" ,
(char*) "turquoise" ,
(char*) "cyan" ,
(char*) "light" ,
(char*) "LightCyan" ,
(char*) "cadet" ,
(char*) "CadetBlue" ,
(char*) "medium" ,
(char*) "MediumAquamarine" ,
(char*) "aquamarine" ,
(char*) "dark" ,
(char*) "DarkGreen" ,
(char*) "dark" ,
(char*) "DarkOliveGreen" ,
(char*) "dark" ,
(char*) "DarkSeaGreen" ,
(char*) "sea" ,
(char*) "SeaGreen" ,
(char*) "medium" ,
(char*) "MediumSeaGreen" ,
(char*) "light" ,
(char*) "LightSeaGreen" ,
(char*) "pale" ,
(char*) "PaleGreen" ,
(char*) "spring" ,
(char*) "SpringGreen" ,
(char*) "lawn" ,
(char*) "LawnGreen" ,
(char*) "green" ,
(char*) "chartreuse" ,
(char*) "medium" ,
(char*) "MediumSpringGreen" ,
(char*) "green" ,
(char*) "GreenYellow" ,
(char*) "lime" ,
(char*) "LimeGreen" ,
(char*) "yellow" ,
(char*) "YellowGreen" ,
(char*) "forest" ,
(char*) "ForestGreen" ,
(char*) "olive" ,
(char*) "OliveDrab" ,
(char*) "dark" ,
(char*) "DarkKhaki" ,
(char*) "khaki" ,
(char*) "pale" ,
(char*) "PaleGoldenrod" ,
(char*) "light" ,
(char*) "LightGoldenrodYellow" ,
(char*) "light" ,
(char*) "LightYellow" ,
(char*) "yellow" ,
(char*) "gold" ,
(char*) "light" ,
(char*) "LightGoldenrod" ,
(char*) "goldenrod" ,
(char*) "dark" ,
(char*) "DarkGoldenrod" ,
(char*) "rosy" ,
(char*) "RosyBrown" ,
(char*) "indian" ,
(char*) "IndianRed" ,
(char*) "saddle" ,
(char*) "SaddleBrown" ,
(char*) "sienna" ,
(char*) "peru" ,
(char*) "burlywood" ,
(char*) "beige" ,
(char*) "wheat" ,
(char*) "sandy" ,
(char*) "SandyBrown" ,
(char*) "tan" ,
(char*) "chocolate" ,
(char*) "firebrick" ,
(char*) "brown" ,
(char*) "dark" ,
(char*) "DarkSalmon" ,
(char*) "salmon" ,
(char*) "light" ,
(char*) "LightSalmon" ,
(char*) "orange" ,
(char*) "dark" ,
(char*) "DarkOrange" ,
(char*) "coral" ,
(char*) "light" ,
(char*) "LightCoral" ,
(char*) "tomato" ,
(char*) "orange" ,
(char*) "OrangeRed" ,
(char*) "red" ,
(char*) "hot" ,
(char*) "HotPink" ,
(char*) "deep" ,
(char*) "DeepPink" ,
(char*) "pink" ,
(char*) "light" ,
(char*) "LightPink" ,
(char*) "pale" ,
(char*) "PaleVioletRed" ,
(char*) "maroon" ,
(char*) "medium" ,
(char*) "MediumVioletRed" ,
(char*) "violet" ,
(char*) "VioletRed" ,
(char*) "magenta" ,
(char*) "violet" ,
(char*) "plum" ,
(char*) "orchid" ,
(char*) "medium" ,
(char*) "MediumOrchid" ,
(char*) "dark" ,
(char*) "DarkOrchid" ,
(char*) "dark" ,
(char*) "DarkViolet" ,
(char*) "blue" ,
(char*) "BlueViolet" ,
(char*) "purple" ,
(char*) "medium" ,
(char*) "MediumPurple" ,
(char*) "thistle" ,
(char*) "snow1" ,
(char*) "snow2" ,
(char*) "snow3" ,
(char*) "snow4" ,
(char*) "seashell1" ,
(char*) "seashell2" ,
(char*) "seashell3" ,
(char*) "seashell4" ,
(char*) "AntiqueWhite1" ,
(char*) "AntiqueWhite2" ,
(char*) "AntiqueWhite3" ,
(char*) "AntiqueWhite4" ,
(char*) "bisque1" ,
(char*) "bisque2" ,
(char*) "bisque3" ,
(char*) "bisque4" ,
(char*) "PeachPuff1" ,
(char*) "PeachPuff2" ,
(char*) "PeachPuff3" ,
(char*) "PeachPuff4" ,
(char*) "NavajoWhite1" ,
(char*) "NavajoWhite2" ,
(char*) "NavajoWhite3" ,
(char*) "NavajoWhite4" ,
(char*) "LemonChiffon1" ,
(char*) "LemonChiffon2" ,
(char*) "LemonChiffon3" ,
(char*) "LemonChiffon4" ,
(char*) "cornsilk1" ,
(char*) "cornsilk2" ,
(char*) "cornsilk3" ,
(char*) "cornsilk4" ,
(char*) "ivory1" ,
(char*) "ivory2" ,
(char*) "ivory3" ,
(char*) "ivory4" ,
(char*) "honeydew1" ,
(char*) "honeydew2" ,
(char*) "honeydew3" ,
(char*) "honeydew4" ,
(char*) "LavenderBlush1" ,
(char*) "LavenderBlush2" ,
(char*) "LavenderBlush3" ,
(char*) "LavenderBlush4" ,
(char*) "MistyRose1" ,
(char*) "MistyRose2" ,
(char*) "MistyRose3" ,
(char*) "MistyRose4" ,
(char*) "azure1" ,
(char*) "azure2" ,
(char*) "azure3" ,
(char*) "azure4" ,
(char*) "SlateBlue1" ,
(char*) "SlateBlue2" ,
(char*) "SlateBlue3" ,
(char*) "SlateBlue4" ,
(char*) "RoyalBlue1" ,
(char*) "RoyalBlue2" ,
(char*) "RoyalBlue3" ,
(char*) "RoyalBlue4" ,
(char*) "blue1" ,
(char*) "blue2" ,
(char*) "blue3" ,
(char*) "blue4" ,
(char*) "DodgerBlue1" ,
(char*) "DodgerBlue2" ,
(char*) "DodgerBlue3" ,
(char*) "DodgerBlue4" ,
(char*) "SteelBlue1" ,
(char*) "SteelBlue2" ,
(char*) "SteelBlue3" ,
(char*) "SteelBlue4" ,
(char*) "DeepSkyBlue1" ,
(char*) "DeepSkyBlue2" ,
(char*) "DeepSkyBlue3" ,
(char*) "DeepSkyBlue4" ,
(char*) "SkyBlue1" ,
(char*) "SkyBlue2" ,
(char*) "SkyBlue3" ,
(char*) "SkyBlue4" ,
(char*) "LightSkyBlue1" ,
(char*) "LightSkyBlue2" ,
(char*) "LightSkyBlue3" ,
(char*) "LightSkyBlue4" ,
(char*) "SlateGray1" ,
(char*) "SlateGray2" ,
(char*) "SlateGray3" ,
(char*) "SlateGray4" ,
(char*) "LightSteelBlue1" ,
(char*) "LightSteelBlue2" ,
(char*) "LightSteelBlue3" ,
(char*) "LightSteelBlue4" ,
(char*) "LightBlue1" ,
(char*) "LightBlue2" ,
(char*) "LightBlue3" ,
(char*) "LightBlue4" ,
(char*) "LightCyan1" ,
(char*) "LightCyan2" ,
(char*) "LightCyan3" ,
(char*) "LightCyan4" ,
(char*) "PaleTurquoise1" ,
(char*) "PaleTurquoise2" ,
(char*) "PaleTurquoise3" ,
(char*) "PaleTurquoise4" ,
(char*) "CadetBlue1" ,
(char*) "CadetBlue2" ,
(char*) "CadetBlue3" ,
(char*) "CadetBlue4" ,
(char*) "turquoise1" ,
(char*) "turquoise2" ,
(char*) "turquoise3" ,
(char*) "turquoise4" ,
(char*) "cyan1" ,
(char*) "cyan2" ,
(char*) "cyan3" ,
(char*) "cyan4" ,
(char*) "DarkSlateGray1" ,
(char*) "DarkSlateGray2" ,
(char*) "DarkSlateGray3" ,
(char*) "DarkSlateGray4" ,
(char*) "aquamarine1" ,
(char*) "aquamarine2" ,
(char*) "aquamarine3" ,
(char*) "aquamarine4" ,
(char*) "DarkSeaGreen1" ,
(char*) "DarkSeaGreen2" ,
(char*) "DarkSeaGreen3" ,
(char*) "DarkSeaGreen4" ,
(char*) "SeaGreen1" ,
(char*) "SeaGreen2" ,
(char*) "SeaGreen3" ,
(char*) "SeaGreen4" ,
(char*) "PaleGreen1" ,
(char*) "PaleGreen2" ,
(char*) "PaleGreen3" ,
(char*) "PaleGreen4" ,
(char*) "SpringGreen1" ,
(char*) "SpringGreen2" ,
(char*) "SpringGreen3" ,
(char*) "SpringGreen4" ,
(char*) "green1" ,
(char*) "green2" ,
(char*) "green3" ,
(char*) "green4" ,
(char*) "chartreuse1" ,
(char*) "chartreuse2" ,
(char*) "chartreuse3" ,
(char*) "chartreuse4" ,
(char*) "OliveDrab1" ,
(char*) "OliveDrab2" ,
(char*) "OliveDrab3" ,
(char*) "OliveDrab4" ,
(char*) "DarkOliveGreen1" ,
(char*) "DarkOliveGreen2" ,
(char*) "DarkOliveGreen3" ,
(char*) "DarkOliveGreen4" ,
(char*) "khaki1" ,
(char*) "khaki2" ,
(char*) "khaki3" ,
(char*) "khaki4" ,
(char*) "LightGoldenrod1" ,
(char*) "LightGoldenrod2" ,
(char*) "LightGoldenrod3" ,
(char*) "LightGoldenrod4" ,
(char*) "LightYellow1" ,
(char*) "LightYellow2" ,
(char*) "LightYellow3" ,
(char*) "LightYellow4" ,
(char*) "yellow1" ,
(char*) "yellow2" ,
(char*) "yellow3" ,
(char*) "yellow4" ,
(char*) "gold1" ,
(char*) "gold2" ,
(char*) "gold3" ,
(char*) "gold4" ,
(char*) "goldenrod1" ,
(char*) "goldenrod2" ,
(char*) "goldenrod3" ,
(char*) "goldenrod4" ,
(char*) "DarkGoldenrod1" ,
(char*) "DarkGoldenrod2" ,
(char*) "DarkGoldenrod3" ,
(char*) "DarkGoldenrod4" ,
(char*) "RosyBrown1" ,
(char*) "RosyBrown2" ,
(char*) "RosyBrown3" ,
(char*) "RosyBrown4" ,
(char*) "IndianRed1" ,
(char*) "IndianRed2" ,
(char*) "IndianRed3" ,
(char*) "IndianRed4" ,
(char*) "sienna1" ,
(char*) "sienna2" ,
(char*) "sienna3" ,
(char*) "sienna4" ,
(char*) "burlywood1" ,
(char*) "burlywood2" ,
(char*) "burlywood3" ,
(char*) "burlywood4" ,
(char*) "wheat1" ,
(char*) "wheat2" ,
(char*) "wheat3" ,
(char*) "wheat4" ,
(char*) "tan1" ,
(char*) "tan2" ,
(char*) "tan3" ,
(char*) "tan4" ,
(char*) "chocolate1" ,
(char*) "chocolate2" ,
(char*) "chocolate3" ,
(char*) "chocolate4" ,
(char*) "firebrick1" ,
(char*) "firebrick2" ,
(char*) "firebrick3" ,
(char*) "firebrick4" ,
(char*) "brown1" ,
(char*) "brown2" ,
(char*) "brown3" ,
(char*) "brown4" ,
(char*) "salmon1" ,
(char*) "salmon2" ,
(char*) "salmon3" ,
(char*) "salmon4" ,
(char*) "LightSalmon1" ,
(char*) "LightSalmon2" ,
(char*) "LightSalmon3" ,
(char*) "LightSalmon4" ,
(char*) "orange1" ,
(char*) "orange2" ,
(char*) "orange3" ,
(char*) "orange4" ,
(char*) "DarkOrange1" ,
(char*) "DarkOrange2" ,
(char*) "DarkOrange3" ,
(char*) "DarkOrange4" ,
(char*) "coral1" ,
(char*) "coral2" ,
(char*) "coral3" ,
(char*) "coral4" ,
(char*) "tomato1" ,
(char*) "tomato2" ,
(char*) "tomato3" ,
(char*) "tomato4" ,
(char*) "OrangeRed1" ,
(char*) "OrangeRed2" ,
(char*) "OrangeRed3" ,
(char*) "OrangeRed4" ,
(char*) "red1" ,
(char*) "red2" ,
(char*) "red3" ,
(char*) "red4" ,
(char*) "DeepPink1" ,
(char*) "DeepPink2" ,
(char*) "DeepPink3" ,
(char*) "DeepPink4" ,
(char*) "HotPink1" ,
(char*) "HotPink2" ,
(char*) "HotPink3" ,
(char*) "HotPink4" ,
(char*) "pink1" ,
(char*) "pink2" ,
(char*) "pink3" ,
(char*) "pink4" ,
(char*) "LightPink1" ,
(char*) "LightPink2" ,
(char*) "LightPink3" ,
(char*) "LightPink4" ,
(char*) "PaleVioletRed1" ,
(char*) "PaleVioletRed2" ,
(char*) "PaleVioletRed3" ,
(char*) "PaleVioletRed4" ,
(char*) "maroon1" ,
(char*) "maroon2" ,
(char*) "maroon3" ,
(char*) "maroon4" ,
(char*) "VioletRed1" ,
(char*) "VioletRed2" ,
(char*) "VioletRed3" ,
(char*) "VioletRed4" ,
(char*) "magenta1" ,
(char*) "magenta2" ,
(char*) "magenta3" ,
(char*) "magenta4" ,
(char*) "orchid1" ,
(char*) "orchid2" ,
(char*) "orchid3" ,
(char*) "orchid4" ,
(char*) "plum1" ,
(char*) "plum2" ,
(char*) "plum3" ,
(char*) "plum4" ,
(char*) "MediumOrchid1" ,
(char*) "MediumOrchid2" ,
(char*) "MediumOrchid3" ,
(char*) "MediumOrchid4" ,
(char*) "DarkOrchid1" ,
(char*) "DarkOrchid2" ,
(char*) "DarkOrchid3" ,
(char*) "DarkOrchid4" ,
(char*) "purple1" ,
(char*) "purple2" ,
(char*) "purple3" ,
(char*) "purple4" ,
(char*) "MediumPurple1" ,
(char*) "MediumPurple2" ,
(char*) "MediumPurple3" ,
(char*) "MediumPurple4" ,
(char*) "thistle1" ,
(char*) "thistle2" ,
(char*) "thistle3" ,
(char*) "thistle4" ,
(char*) "gray0" ,
(char*) "grey0" ,
(char*) "gray1" ,
(char*) "grey1" ,
(char*) "gray2" ,
(char*) "grey2" ,
(char*) "gray3" ,
(char*) "grey3" ,
(char*) "gray4" ,
(char*) "grey4" ,
(char*) "gray5" ,
(char*) "grey5" ,
(char*) "gray6" ,
(char*) "grey6" ,
(char*) "gray7" ,
(char*) "grey7" ,
(char*) "gray8" ,
(char*) "grey8" ,
(char*) "gray9" ,
(char*) "grey9" ,
(char*) "gray10" ,
(char*) "grey10" ,
(char*) "gray11" ,
(char*) "grey11" ,
(char*) "gray12" ,
(char*) "grey12" ,
(char*) "gray13" ,
(char*) "grey13" ,
(char*) "gray14" ,
(char*) "grey14" ,
(char*) "gray15" ,
(char*) "grey15" ,
(char*) "gray16" ,
(char*) "grey16" ,
(char*) "gray17" ,
(char*) "grey17" ,
(char*) "gray18" ,
(char*) "grey18" ,
(char*) "gray19" ,
(char*) "grey19" ,
(char*) "gray20" ,
(char*) "grey20" ,
(char*) "gray21" ,
(char*) "grey21" ,
(char*) "gray22" ,
(char*) "grey22" ,
(char*) "gray23" ,
(char*) "grey23" ,
(char*) "gray24" ,
(char*) "grey24" ,
(char*) "gray25" ,
(char*) "grey25" ,
(char*) "gray26" ,
(char*) "grey26" ,
(char*) "gray27" ,
(char*) "grey27" ,
(char*) "gray28" ,
(char*) "grey28" ,
(char*) "gray29" ,
(char*) "grey29" ,
(char*) "gray30" ,
(char*) "grey30" ,
(char*) "gray31" ,
(char*) "grey31" ,
(char*) "gray32" ,
(char*) "grey32" ,
(char*) "gray33" ,
(char*) "grey33" ,
(char*) "gray34" ,
(char*) "grey34" ,
(char*) "gray35" ,
(char*) "grey35" ,
(char*) "gray36" ,
(char*) "grey36" ,
(char*) "gray37" ,
(char*) "grey37" ,
(char*) "gray38" ,
(char*) "grey38" ,
(char*) "gray39" ,
(char*) "grey39" ,
(char*) "gray40" ,
(char*) "grey40" ,
(char*) "gray41" ,
(char*) "grey41" ,
(char*) "gray42" ,
(char*) "grey42" ,
(char*) "gray43" ,
(char*) "grey43" ,
(char*) "gray44" ,
(char*) "grey44" ,
(char*) "gray45" ,
(char*) "grey45" ,
(char*) "gray46" ,
(char*) "grey46" ,
(char*) "gray47" ,
(char*) "grey47" ,
(char*) "gray48" ,
(char*) "grey48" ,
(char*) "gray49" ,
(char*) "grey49" ,
(char*) "gray50" ,
(char*) "grey50" ,
(char*) "gray51" ,
(char*) "grey51" ,
(char*) "gray52" ,
(char*) "grey52" ,
(char*) "gray53" ,
(char*) "grey53" ,
(char*) "gray54" ,
(char*) "grey54" ,
(char*) "gray55" ,
(char*) "grey55" ,
(char*) "gray56" ,
(char*) "grey56" ,
(char*) "gray57" ,
(char*) "grey57" ,
(char*) "gray58" ,
(char*) "grey58" ,
(char*) "gray59" ,
(char*) "grey59" ,
(char*) "gray60" ,
(char*) "grey60" ,
(char*) "gray61" ,
(char*) "grey61" ,
(char*) "gray62" ,
(char*) "grey62" ,
(char*) "gray63" ,
(char*) "grey63" ,
(char*) "gray64" ,
(char*) "grey64" ,
(char*) "gray65" ,
(char*) "grey65" ,
(char*) "gray66" ,
(char*) "grey66" ,
(char*) "gray67" ,
(char*) "grey67" ,
(char*) "gray68" ,
(char*) "grey68" ,
(char*) "gray69" ,
(char*) "grey69" ,
(char*) "gray70" ,
(char*) "grey70" ,
(char*) "gray71" ,
(char*) "grey71" ,
(char*) "gray72" ,
(char*) "grey72" ,
(char*) "gray73" ,
(char*) "grey73" ,
(char*) "gray74" ,
(char*) "grey74" ,
(char*) "gray75" ,
(char*) "grey75" ,
(char*) "gray76" ,
(char*) "grey76" ,
(char*) "gray77" ,
(char*) "grey77" ,
(char*) "gray78" ,
(char*) "grey78" ,
(char*) "gray79" ,
(char*) "grey79" ,
(char*) "gray80" ,
(char*) "grey80" ,
(char*) "gray81" ,
(char*) "grey81" ,
(char*) "gray82" ,
(char*) "grey82" ,
(char*) "gray83" ,
(char*) "grey83" ,
(char*) "gray84" ,
(char*) "grey84" ,
(char*) "gray85" ,
(char*) "grey85" ,
(char*) "gray86" ,
(char*) "grey86" ,
(char*) "gray87" ,
(char*) "grey87" ,
(char*) "gray88" ,
(char*) "grey88" ,
(char*) "gray89" ,
(char*) "grey89" ,
(char*) "gray90" ,
(char*) "grey90" ,
(char*) "gray91" ,
(char*) "grey91" ,
(char*) "gray92" ,
(char*) "grey92" ,
(char*) "gray93" ,
(char*) "grey93" ,
(char*) "gray94" ,
(char*) "grey94" ,
(char*) "gray95" ,
(char*) "grey95" ,
(char*) "gray96" ,
(char*) "grey96" ,
(char*) "gray97" ,
(char*) "grey97" ,
(char*) "gray98" ,
(char*) "grey98" ,
(char*) "gray99" ,
(char*) "grey99" ,
(char*) "gray100" ,
(char*) "grey100" ,
(char*) "dark" ,
(char*) "DarkGrey" ,
(char*) "dark" ,
(char*) "DarkGray" ,
(char*) "dark" ,
(char*) "DarkBlue" ,
(char*) "dark" ,
(char*) "DarkCyan" ,
(char*) "dark" ,
(char*) "DarkMagenta" ,
(char*) "dark" ,
(char*) "DarkRed" ,
(char*) "light" ,
(char*) "LightGreen" 

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










