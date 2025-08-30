#include <cmath>
#include "Xgraphics.h"
#include <iostream>

using namespace ::std ;
//#define PI 3.14159265
//Zur Verwaltung der Fenster benotigte Variablen:

Window ourwindow;
World ourworld;
XEvent ourevent;

void drawgal(float t, int nring, float v);
//void initial(int active_r [], int active_a [], int cluster[] [ninit+1], int &nactive);


//Hauptprogramm:
main() 
{  
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

float 
  //a_0, Anfangsradius für die ersten 6 Kreise
  t = 0., //Zeitvariable
  dt = 0.001, //Zeitschritt
  nring = 50, // Anznahl der Ringe  
  v = 2000. //Geschweindigkeit
  ;
int done = 0; /* control variable */;

// a_0 = r*(1.-sin(M_PI/n))/sin(M_PI/n);  Anfangsradius für die ersten 6 Kreise

while(!done) 
{
  GetEvent(&ourevent,0);
  //init = init +1;

  ClearWorld(ourworld);

  t = t + dt; //nächsten Zeitschritt berechnen

  drawgal(t, nring, v); // aufrufen der Zeichnenfunktion

  //delta_phi = delta_phi + 1; // für konstante Kreisfrequenz

  system ("sleep 0.01");

  if (ourevent.type == ButtonPress) //Event-Behandlung: Warten auf einen Mausklick.
    done =1;
}
  //Sauberes Beenden des Programms:
  ExitX();
  return 0;/* clean up memory */
}



void drawgal(float t, int nring, float v)
{
  float 
  r = 0.4,
  a,
   // phi, Winkelverschiebung/distanz
  // delta_phi = 0., 
  x ,y,
  w //Kreisfrequenz
  ;
  int n = 6, l, i, k; /* control variable N,*/;
  
  WFillCircle(ourworld, 0, 0, r, 50); //Mittelkreis zeichnen
  
  for(l = 0; l < 2; ++l) //
  {
    for(i = 0; i <= nring; ++i)
    {
      //N = n*i; Anzahl der Kreise auf dem i. Ring
      //phi = 360./N; Winkeldistanz von jedem einzelnem Ring
      a = r * (1. - sin(M_PI / (n * i))) / sin(M_PI / (n * i)); //Berechnung des neuen Abstands
      w = v / a; 
      /*if (init != 1)
      {
      delta_phi = (a_0 +r )/(a + r) + delta_phi; für konstante Kreisgeschwindigkeit
      //system ("sleep 0.001");
      }
      else delta_phi = (a_0 +r )/(a + r);*/
      for(k = 0; k < (n * i); ++k)
      {
	x = (a + r) * cos(2. * k * M_PI / (n * i) + w * t);//Umrechnung von Polar in Kartesischen Koordianten- delta_phi
	y = (a + r) * sin(2. * k * M_PI / (n * i) + w * t);//- delta_phi
	if (fmod(k,i) == 0) // wenn k ein vielfaches von i ist dann färbe es blau
	{
	  WFillCircle(ourworld, x, y, r, 50); //Kreis zeichnen
	}
	else //ansonsten schwarz
	{
	  //cout << "x = " << x << " y = " << y << endl; // Koodiantenausgabe
	  WDrawCircle(ourworld, x, y, r, 1); //Kreis zeichnen
	}
      }
    };
  };//
}


