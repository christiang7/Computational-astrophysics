#include <cmath>

// Euler-Cromer algorithm for Kepler problem

void euler (float pos[], float vel[], float &t, float const &GM, float &dt, float &P)
{
  float r2, r3, accel[2] ;
  
  // radial distance (coupling of x and y)
  r2 = pos[0] * pos[0] + pos[1] * pos[1] ;
  r3 = r2 * sqrt(r2) ;
  
  // cartesian coordinates x and y
  for (short i = 0 ; i < 2 ; i++)
    {
      accel[i] = -1. * GM * pos[i] / r3 ;
      vel[i]   = vel[i] + accel[i] * dt ;
      pos[i]   = pos[i] + vel[i] * dt ;

    }

  // incrementing time
  t = t + dt ;
  P = sqrt(r3);
}
