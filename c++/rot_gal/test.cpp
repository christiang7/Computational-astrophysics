#include <cmath>
#include "Xgraphics.h"
#include <iostream>

using namespace ::std ;
//#define PI 3.14159265
//Zur Verwaltung der Fenster benotigte Variablen:

Window ourwindow;
World ourworld;
XEvent ourevent;

void drawgal(float t,int nring);

//Hauptprogramm:
main() 
{
float 
  a_0,
  t = 0., //Zeitvariable
  dt = 0.001, //Zeitschritt
  nring = 20
  ;
int done = 0; /* control variable */;
  
  
InitX(); /* connect to X-server */

/* set up windows and worlds */
ourwindow = CreateWindow(800,800,"demo1");
ourworld = CreateWorld(ourwindow,0,0,800,800,-40,40,40,-40,0,0);
/* width, height
/* name of window
/*
window to draw in
x,y,width,height
wx1, wy1, wx2, wy2
scaling property
"gravity"
*/

//Darstellen des Fensters und Zeichnen der Kreise:
ShowWindow(ourwindow);


// a_0 = r*(1.-sin(M_PI/n))/sin(M_PI/n);  Anfangsradius für die ersten 6 Kreise

while(!done) {
GetEvent(&ourevent,0);

//init = init +1;

ClearWorld(ourworld);

t = t+dt; //nächsten Zeitschritt berechnen

drawgal(t,nring);

//delta_phi = delta_phi + 1; // für konstante Kreisfrequenz
 system ("sleep 0.1");


if (ourevent.type == ButtonPress) done =1;}//Event-Behandlung: Warten auf einen Mausklick.
//Sauberes Beenden des Programms:
ExitX();
return 0;/* clean up memory */
}

void drawgal(float t, int nring)
{
  float 
  r=1,
  a,
  phi,
  delta_phi = 0.,
  x ,y,
  w, //Kreisfrequenz
  v =200. //Geschweindigkeit
 //Zeitvariable
  ;
  int n = 6,N, l, i, k; /* control variable */;
  
  WFillCircle(ourworld,0,0,r,50); //Kreis zeichnen
  
  for(l=0;l<2;++l){//
    for(i=0;i<=nring;++i)
    {
      N = n*i; //Anzahl der Kreise auf dem i. Ring
      phi = 360./N; //Winkeldistanz von jedem einzelnem Ring
      a = r*(1.-sin(M_PI/N))/sin(M_PI/N); //Berechnung des neuen Abstands
      w = v/a; 
      /*if (init != 1)
      {
      delta_phi = (a_0 +r )/(a + r) + delta_phi; für konstante Kreisgeschwindigkeit
      //system ("sleep 0.001");
      }
      else delta_phi = (a_0 +r )/(a + r);*/
      for(k=0;k<N;++k)
      {
	x = (a+r)*cos(phi*k*M_PI/180.0 + w*t);//Umrechnung von Polar in Kartesischen Koordianten- delta_phi
	y = (a+r)*sin(phi*k*M_PI/180.0 + w*t);//- delta_phi
	if (fmod(k,i) == 0) // wenn k ein vielfaches von i ist dann färbe es blau
	{
	  WFillCircle(ourworld,x,y,r,50); //Kreis zeichnen
	}
	else //ansonsten schwarz
	{
	  //cout << "x = " << x << " y = " << y << endl; // Koodiantenausgabe
	  WDrawCircle(ourworld,x,y,r,1); //Kreis zeichnen
	}
      }
    };
  };//
}


