#include <cmath>

int const np = 2 ;
int const nd = 2 ;

// Euler-Richardson algorithm for Kepler problem

void euler (double pos[np][nd], double vel[np][nd], double &t, double const &GM, 
            double &dt, double ratio[np])
{
  double r2, r3, accel, dpos[nd], dr2, dr, dr3 ;
  double pos2[np][nd], vel2[np][nd] ;

  // ----------------------- step 1 -----------------------------------  
  // distance between planet 1 and planet 2
  for (short j = 0 ; j < nd ; j++)
    {
      dpos[j] = pos[1][j] - pos[0][j] ;
    }
  dr2 = dpos[0]*dpos[0] + dpos[1]*dpos[1] ;
  dr  = sqrt(dr2) ;
  dr3 = dr2 * dr ; 

  for (short i = 0 ; i < np ; i++)
    {
      // radial distance (coupling of x and y)
      r2 = pos[i][0] * pos[i][0] + pos[i][1] * pos[i][1] ;
      r3 = r2 * sqrt(r2) ;
      
      // cartesian coordinates x and y
      for (short j = 0 ; j < nd ; j++)
        {
          accel     = -1. * GM * pos[i][j] / r3  + ratio[i] * dpos[j] / dr3 ;
          vel2[i][j] = vel[i][j] + accel * 0.5* dt ;
          pos2[i][j] = pos[i][j] + vel[i][j] * 0.5 * dt ;

        }
    }
  // ------------------ step 2 ---------------------------------
  // distance between planet 1 and planet 2
  for (short j = 0 ; j < nd ; j++)
    {
      dpos[j] = pos2[1][j] - pos2[0][j] ;
    }
  dr2 = dpos[0]*dpos[0] + dpos[1]*dpos[1] ;
  dr  = sqrt(dr2) ;
  dr3 = dr2 * dr ; 

  for (short i = 0 ; i < np ; i++)
    {
      // radial distance (coupling of x and y)
      r2 = pos2[i][0] * pos2[i][0] + pos2[i][1] * pos2[i][1] ;
      r3 = r2 * sqrt(r2) ;
      
      // cartesian coordinates x and y
      for (short j = 0 ; j < nd ; j++)
        {
          accel     = -1. * GM * pos2[i][j] / r3  + ratio[i] * dpos[j] / dr3 ;
          vel[i][j] = vel[i][j] + accel * dt ;
          pos[i][j] = pos[i][j] + vel2[i][j] * dt ;

        }
    }
  // incrementing time
  t = t + dt ;

}
