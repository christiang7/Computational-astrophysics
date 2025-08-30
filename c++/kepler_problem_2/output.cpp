#include "Xgraphics.h"

// animation of the Kepler problem with help of Xgraphics library

//------------------------------------------------------------------------------------

void output (float pos[], float &t, World &ourworld) 
{
  float plotsize = 0.1 ; // symbol size of Sun

  // plot sun (according to \odot ) in color yellow
  WDrawCircle(ourworld, 0., 0., plotsize , 95 ) ;
  WFillCircle(ourworld, 0., 0., plotsize*.3 , 95 ) ;

  plotsize = 0.01 ; // symbol size of planet

  // plot planet (dot) in color blue
  WFillCircle(ourworld, pos[0] , pos[1] , plotsize , 200 ) ;

}
//------------------------------------------------------------------------------------
