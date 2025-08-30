#include <cmath>

int const nd = 2 ; // number of spacial dimensions

// --- radial distance between points pos and pos2 ---------------------------
double rd (double pos[], double pos2[])
{

  double r2 = 0., r3 ;

  for (short i = 0 ; i < nd ; i++)
    {
      r2 = r2 + (pos[i]-pos2[i])*(pos[i]-pos2[i]);
    }
  
  r3 = r2 * sqrt (r2) ;
  
  return r3 ;
}

// --- derivatives for Runge-Kutta method -----------------------------------
void derivs (double pos[], double vel[], double const &dt, double const &GM,
             double kx[], double kv[])
{
  double accel, 
         pos0[nd] = {0.} ; // for distance to (0;0)

  for (short i = 0 ; i < nd ; i++)
    {
      accel = -1. * GM * pos[i] / rd (pos, pos0) ;
      kv[i] = accel * dt ;
      kx[i] = vel[i] * dt ;
    }
}

// --- Runge-Kutta method for Kepler problem -------------------------------

void runge_kutta (double pos[], double vel[], double &t, double const &GM, double &dt)
{
  double r2, r3, accel ;
  double k1v[nd], k1x[nd], 
         k2v[nd], k2x[nd], 
         k3v[nd], k3x[nd], 
         k4v[nd], k4x[nd] ;
  double posnew[nd] = {0.}, velnew[nd] ={0.} ;
  
  // 1st term k1 --------------------
  derivs(pos, vel, dt, GM, k1x, k1v) ;

  // 2nd term k2 --------------------
  for (short i = 0 ; i < nd ; i++)
    {
      posnew[i] = pos[i] + k1x[i] * 0.5 ;
      velnew[i] = vel[i] + k1v[i] * 0.5 ;
    }
  derivs(posnew, velnew, dt, GM, k2x, k2v) ;
         
  // 3rd term k3 ---------------------
  for (short i = 0 ; i < nd ; i++)
    {
      posnew[i] = pos[i] + k2x[i] * 0.5 ;
      velnew[i] = vel[i] + k2v[i] * 0.5 ;
    } 
  derivs(posnew, velnew, dt, GM, k3x, k3v) ;

  // 4th term k4 -------------------
  for (short i = 0 ; i < nd ; i++)
    {
      posnew[i] = pos[i] + k3x[i] ;
      velnew[i] = vel[i] + k3v[i] ;
    }
  derivs(posnew, velnew, dt, GM, k4x, k4v) ;

  // finally ----------------------
  for (short i = 0 ; i < nd ; i++)
    {      
      vel[i]   = vel[i] + (k1v[i] + 2.*k2v[i] + 2.*k3v[i] + k4v[i])/6. ;
      pos[i]   = pos[i] + (k1x[i] + 2.*k2x[i] + 2.*k3x[i] + k4x[i])/6. ;
    }

  // incrementing time
  t = t + dt ;

}
