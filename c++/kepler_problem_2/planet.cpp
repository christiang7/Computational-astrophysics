#include <iostream>
#include <cmath>
#include "Xgraphics.h"

using namespace std ;

// program for solving the Kepler problem: 1 planet orbiting Sun

void  initial (float pos[], float vel[], float &t, float &GM, float &dt, int &nplot) ;
void  output  (float pos[], float &t, World &ourworld) ;
void  euler   (float pos[], float vel[], float &t, float const &GM, float &dt, float &P) ;
float energy  (float pos[], float vel[], float const &GM) ;

//-----------------------------------------------------------------------------
int main ()
{
  // Xgraphics elements are only localy defined -> function parameters
  Window ourwindow ;
  World  ourworld  ;
  XEvent ourevent  ;
  
  // loop control 
  int counter = 0 , nplot, done = 0 ;
  // arrays to store position and velocity of planet
  float pos[2],
    vel[2],
    t, 
    GM, 
    dt, 
    rmax, 
    P, //Umlaufzeit
    aphel, perihel, a, e, b //Variablen für die Ellipse
    ; 

  // window pixels  
  int  windowx = 600 , windowy = 600 ;
  int  world_xmax, world_ymax, borderx = 5, bordery = 5  ; 

  InitX () ;

  // initializing settings
  initial (pos, vel, t, GM, dt, nplot) ;
  rmax      = 2 * pos[0] ; // maximum value of semi major axis 
 
  // create window and show it
  ourwindow  = CreateWindow(windowx, windowy, (char*)"Sun + Planet") ;
  world_xmax = windowx - 2 * borderx ;
  world_ymax = windowy - 2 * bordery ;
  ourworld   = CreateWorld(ourwindow, borderx, bordery,
                         world_xmax, world_ymax, 
                         -rmax, rmax, rmax, -rmax, 0, 0);  
  ShowWindow(ourwindow) ;
 
  // black background (Space)
  WFillRectangle(ourworld, -rmax, rmax, rmax, -rmax, 1) ;

  // plot with intial settings
  output (pos, t, ourworld) ;

  // wait for mouse button press to stop
  while(!done) {
    GetEvent(&ourevent,0);
    if (ourevent.type == ButtonPress) done = 1 ;
    
    if ( vel[1] == 0 )
    {
      if (vel[0] < 0)
      {
	perihel = pos[0];
      }
      else aphel = pos[0];
      a = (aphel + perihel) / 2;
      e = (aphel * 2 / (aphel + perihel)) - 1;
      b = sqrt(e * e + a * a);
      cout << "a = " << a << "b = " << b << "e = " << e << endl ;
    };
    
    euler (pos, vel, t, GM, dt, P) ;
    
    //cout << "P = " << P << endl ;
    counter++ ;
    if (counter % nplot == 0)
      {
        system ("sleep 0.01") ;
        output (pos, t, ourworld) ;
        //cout << "E_ges/m = " << energy(pos, vel, GM) << endl ;
      }
  }
    
  ExitX() ;
  return 0 ; 
}
//-----------------------------------------------------------------------------
