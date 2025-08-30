#include "Xgraphics.h"

int const np = 2 ;
int const nd = 2 ;

// animation of the Kepler problem with help of Xgraphics library

//------------------------------------------------------------------------------------

void output (double pos[np][nd], double &t, World &ourworld, double pos_old[np][nd], double rmax) 
{
  double plotsize = 0.1 ; // symbol size of Sun

  // plot coordinate cross
  WDrawLine (ourworld, -rmax, 0., +rmax, 0., 0) ;
  WDrawLine (ourworld,   0.,-rmax, 0., +rmax, 0) ;
  // plot scale
  for (short i = 1 ; i < rmax ; i++)
  {
      WDrawLine (ourworld, -double(i), rmax/-100., -double(i), rmax/100., 0) ;
      WDrawLine (ourworld, double(i), rmax/-100., double(i), rmax/100., 0) ;
      WDrawLine (ourworld, rmax/-100., -double(i), rmax/100., -double(i), 0) ;
      WDrawLine (ourworld, rmax/-100., double(i), rmax/100., double(i), 0) ;
  }


  // plot sun (according to \odot ) in color yellow
  WDrawCircle(ourworld, 0., 0., plotsize , 95 ) ;
  WFillCircle(ourworld, 0., 0., plotsize*.3 , 95 ) ;

  plotsize = 0.04 ; // symbol size of planet

  // plot planet (dot) in color blue
  for (short i = 0 ; i < np ; i++)
    {
      WFillCircle(ourworld, pos[i][0] , pos[i][1] , plotsize ,(i+1)*100 ) ;
    }
}
//------------------------------------------------------------------------------------
