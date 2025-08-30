#include <cmath>

// Euler algorithm for Kepler problem

void euler (float pos[], float vel[], float &t, float const &GM, float &dt)
{
  float r2, r3, accel[2] ;
  
  // radial distance (coupling of x and y)
  r2 = pos[0] * pos[0] + pos[1] * pos[1] ;
  r3 = r2 * sqrt(r2) ;
  
  // cartesian coordinates x and y ; beginning of interval
  for (short i = 0 ; i < 2 ; i++)
    {
      accel[i] = -1. * GM * pos[i] / r3 ;
      pos[i]   = pos[i] + vel[i] * dt ;
      vel[i]   = vel[i] + accel[i] * dt ;
    }
 
  // incrementing time
  t = t + dt ;

}
