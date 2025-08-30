#include <cmath>

// Euler-Richardson algorithm for Kepler problem

void euler (float pos[], float vel[], float &t, float const &GM, float &dt)
{
  float r2, r3, accel[2] ;
  float pos_2[2], vel_2[2] ; // Euler-Richardson half steps
  
  // radial distance (coupling of x and y)
  r2 = pos[0] * pos[0] + pos[1] * pos[1] ;
  r3 = r2 * sqrt(r2) ;
  
  // cartesian coordinates x and y ; beginning of interval
  for (short i = 0 ; i < 2 ; i++)
    {
      accel[i] = -1. * GM * pos[i] / r3 ;
      vel_2[i] = vel[i] + accel[i] * dt * 0.5 ; 
      pos_2[i] = pos[i] + vel[i] * dt * 0.5 ; 
    }

  // calculate force for middle of interval
  r2 = pos_2[0] * pos_2[0] + pos_2[1] * pos_2[1] ;
  r3 = r2 * sqrt(r2) ; 

  // 2nd half step
  for (short i = 0 ; i < 2 ; i++)
    {
      accel[i] = -1. * GM * pos_2[i] / r3 ;
      vel[i]   = vel[i] + accel[i] * dt ; 
      pos[i] = pos[i] + vel_2[i] * dt ; 
    }
 
  // incrementing time
  t = t + dt ;

}
