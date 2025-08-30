/*******************************************************
*
Xgraphics demo program
*
*******************************************************/
#include <math.h>
#include "Xgraphics.h"

//Zur Verwaltung der Fenster benotigte Variablen:

Window ourwindow;
World ourworld;
XEvent ourevent;

//Hauptprogramm:
main() {
int i,done = 0;
/* control variable */
//Zunachst mu die Verbindung zum X-Server hergestellt werden:
InitX();
/* connect to X-server */

//Festlegung der Fenster und Zeichenbereiche:
/* set up windows and worlds */
ourwindow = CreateWindow(400,400,"demo1");
ourworld = CreateWorld(ourwindow,5,5,390,390,-1,-1,1,1,0,0);
/* width, height
/* name of window
/*
window to draw in
x,y,width,height
wx1, wy1, wx2, wy2
scaling property
"gravity"
*/

//Darstellen des Fensters und Zeichnen einer Lissajous-Figur:
ShowWindow(ourwindow);
for(i=1;i<620;++i) WDrawCircle(ourworld,cos(i),sin(2*i),0.1,1);
//Event-Behandlung: Warten auf einen Mausklick.
while(!done) {
GetEvent(&ourevent,1);
if (ourevent.type == ButtonPress) done =1;}
//Sauberes Beenden des Programms:
ExitX();
return 0;/* clean up memory */
}
