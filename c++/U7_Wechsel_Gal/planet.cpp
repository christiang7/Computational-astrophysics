#include <iostream>
#include <cmath>
#include "Xgraphics.h"

using namespace std ;

int const np = 2 ; // number of planets
int const nd = 2 ; // number of dimensions

// program for solving the Kepler problem: np planets orbiting Sun

void   initial (double pos[np][nd], double vel[np][nd], double &t, double &GM, double &dt, int &nplot, double ratio[np]) ;
void   output  (double pos[np][nd], double &t, World &ourworld, double pos_old[np][nd], double rmax) ;
void   euler   (double pos[np][nd], double vel[np][nd], double &t, double const &GM, double &dt, double ratio[np]) ;

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
  double pos[np][nd], vel[np][nd], t, GM, dt , rmax ;
  double plotpos_old[np][nd] = {0} ;
  double ratio[np] ; 

  // window pixels  
  int  windowx = 600 , windowy = 600 ;
  int  world_xmax, world_ymax, borderx = 5, bordery = 5  ; 

  InitX () ;

  // initializing settings
  initial (pos, vel, t, GM, dt, nplot, ratio) ;
  rmax      = 2 * pos[1][0] ; // maximum value of semi major axis 

  // create window and show it
  ourwindow  = CreateWindow(windowx, windowy, (char*)"Sun + Planet + Planet") ;
  world_xmax = windowx - 2 * borderx ;
  world_ymax = windowy - 2 * bordery ;
  ourworld   = CreateWorld(ourwindow, borderx, bordery,
                         world_xmax, world_ymax, 
                         -rmax, rmax, rmax, -rmax, 0, 0);  
  ShowWindow(ourwindow) ;
 
  // black background (Space)
  WFillRectangle(ourworld, -rmax, rmax, rmax, -rmax, 1) ;

  // plot with intial settings
  output (pos, t, ourworld, pos, rmax) ;

  // wait for mouse button press to stop
  while(!done) {
    GetEvent(&ourevent,0);
    if (ourevent.type == ButtonPress) done = 1 ;
    
    // integration of the equations of motion
    euler (pos, vel, t, GM, dt, ratio)  ;

    counter++ ; 

    if (counter % nplot == 0)
      {
        system ("sleep 0.01") ;	
        // output for each nplot step of counter
        output (pos, t, ourworld, plotpos_old, rmax) ;
        // copy arrays or use std::copy
	for (short i = 0  ; i < np ; i++)
	  {
            for (short j = 0 ; j < nd ; j++)
              {
                plotpos_old[i][j] = pos[i][j] ;
              }
	  }
        // --- plotpos_old to erase previous plot position
      }
    
  }
    


  ExitX() ;
  return 0 ; 
}
//-----------------------------------------------------------------------------
