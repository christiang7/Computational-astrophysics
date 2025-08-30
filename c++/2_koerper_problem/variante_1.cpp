#include <iostream>
#include <cmath>
#include "Xgraphics.h"

using namespace ::std ;

Window ourwindow;
World ourworld;
XEvent ourevent;

void anfang (float &t, float &x, float &y, float &v_x, float &v_y, float &Delta_t) ;//, float &tmax
void eulerverfahren (float &t, float &x, float &y, float &v_x, float &v_y, float &Delta_t) ;// float &a_x, float &a_y, numerische Berechnung der Lösung der DGL
void output (float &x, float &y); //Bildschirmausgabe

/*
float 
  v_x = 1., //Anfangswert fuer Geschwindigkeit
  v_y = 1., //Anfangswert fuer x-Richt Geschwindigkeit
  x = 1., // Anfangswert fuer x
  y = 1., // Anfangswert fuer y
  a
;*/
//float eulerver [2][2] = [v_x v_y,x y]; //Array des Eulerverfahrens wo die Korrdinaten drin enthalten sind

int main()
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
  // Programm zum Loesen der Dgl. dx/dt = 2 * t
  float t, x, y, v_x, v_y, Delta_t = 0.01 ;// a_x, a_y,, tmax
  int done = 0;

  // Angabe der Startwerte 
  anfang (t, x, y, v_x, v_y, Delta_t) ;//, tmax
  /*
  while (t <= tmax)
    {
      eulerverfahren (t, x, y, v_x, v_y, Delta_t) ;// a_x, a_y,       // einfaches Eulerverfahren
      cout << " t = " <<  t << " x = "  << x << " v_x = " << v_x << " y = " << y << " v_y = " << v_y 
	   << endl ;
      //x = t * t;
      //cout <<  t << " "  << x << "--" << endl ;
    }
  */
  while(!done) 
  {
    GetEvent(&ourevent,0);
    //init = init +1;
    
    system ("sleep 0.01");
    
    ClearWorld(ourworld);

    eulerverfahren (t, x, y, v_x, v_y, Delta_t) ;// a_x, a_y,       // einfaches Eulerverfahren
    cout << " t = " <<  t << " x = "  << x << " v_x = " << v_x << " y = " << y << " v_y = " << v_y 
	 << endl ;
    
    output( x, y);
    
    

    if (ourevent.type == ButtonPress) //Event-Behandlung: Warten auf einen Mausklick.
    done =1;
  }
  //Sauberes Beenden des Programms:
  ExitX();
  return 0;/* clean up memory */
}  

void anfang (float &t, float &x, float &y, float &v_x, float &v_y, float &Delta_t)//, float &tmax
{
  // Initialisieren der Startwerte, Schrittweite und Ausgabe
  t = 0 ;                         // Anfangswert fuer t
  //tmax = 5. ;                      // Maximalwert fuer t
  //Anfangskorrdinaten festlegen
  x = 5. ;
  y = 0. ;
  v_x = 0. ;
  v_y = 2. * M_PI / pow(x, 1 / 2) ; // für Kreisbahnen
  //Delta_t = 0.01 ;                  // Schrittweite in t
  cout << "# t " << " x "  << endl; // Tabellenkopf
  cout << " t_0 = " <<  t  << " x_0 " << x << " v_x,0 = " << v_x << " y,0 = " << y << " v_y,0 = " << v_y << endl ; // Ausgabe der Startwerte
}

void eulerverfahren (float &t, float &x, float &y, float &v_x, float &v_y, float &Delta_t)// float &a_x, float &a_y,
{
  // intervallweise Berechnung von x, jedes Intervall [t,t+Delta_t]

  float a_x, a_y;//anstieg, Delta_x
  a_x = - 4 * pow(M_PI, 2) * x / pow( pow(x, 2) + pow(y, 2), 3 / 2 );
  a_y = - 4 * pow(M_PI, 2) * y / pow( pow(x, 2) + pow(y, 2), 3 / 2 );
  v_x = v_x + a_x * Delta_t;
  x = x + v_x * Delta_t;
  v_y = v_y + a_y * Delta_t;
  y = y + v_y * Delta_t;
  //anstieg = 2. * t ;               // Anstieg am Anfang des Intervalls
  //Delta_x = anstieg * Delta_t ;    // geschaetzte Aenderung auf dem Intervall 
  //x = x + Delta_x ;                // neuer x-Wert
  t = t + Delta_t ;                // t-Wert am Ende des Intervalls
}

void output (float &x, float &y)
{
  float r = 0.6 ; //Größe des Planeten
  for(int l = 0; l < 2; ++l) //
  {
    WFillCircle(ourworld, 0, 0, 1, 350); //Sonne zeichnen
    WFillCircle(ourworld, x, y, r, 30); // Planet zeichnen
  }
}
;