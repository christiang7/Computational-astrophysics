#include <math.h>
#include"Xgraphics.h"

// Zur Verwaltung der Fenter benötigte Variablen:

Window chwindow;
World chworld;
XEvent chevent;

//Hauptprogramm:

main(int argc, char **argv)
{
  int i,done = 0; /*control variable*/
  
// zunächst muss die Verbindung zum X-Server hergestellt werden
  
  InitX(); /*connect to X-Server*/

// Festlegung der Fenster und Zerichenbereiche;

/* set up windows and worlds */

  chwindow = CreateWindow(
			 800,800, /*width, height	*/
			 (char*)"demo1" /*name of wwindow	*/
			 );
  chworld = CreateWorld(
			chwindow, /*window to draw in	*/
			5,5,790,790, /*x,y,width,height	*/
			-1,-1,1,1, /*wx1,wy1,wx2,wy2	*/
			0, /* scaling property		*/
			0 /* "gravity"			*/
			);

// Darstellen des Fenters und Zeichnen einer Lissajous-Figur:

  ShowWindow(chwindow);
  
  for (i=1;i<620;i++) 
    WDrawRectangle(chworld,cos(i),sin(2*i),cos(i+2),sin(2*i+2),i);

// Event-Behandlung: Warten auf einen Mausklick.
  
  while(!done)
  {
    GetEvent(&chevent,1);
    if (chevent.type == ButtonPress) done =1;
  }

// Sauberes Beenden des Programms.
  ExitX();
  return 0;
}