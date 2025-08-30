#include <iostream>
#include <cmath>
#include "Xgraphics.h"

using namespace std ;

Window ourwindow;
World ourworld;
XEvent ourevent;
const int nring = 5;
double x[12+6*6 * nring * (nring + 1)], fac, m_1, m_2, r_2, x_1, y_1, z_1, v_x1, v_y1, v_z1, x_2,
	y_2, z_2, v_x2, v_y2, v_z2, ink_wk1, ink_wk2, phi= 0.7*M_PI;


void initial();
void deriv( double x[], double k, int ndim, double m_1, double m_2);


//-----------------------------------------------------------------------------
int main ()
{
  InitX(); /* connect to X-server */

  /* set up windows and worlds */
  ourwindow = CreateWindow(800,800,(char*)"demo1");
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
  
  //ClearWorld(ourworld);
  
  
  int done = 0;
  while(!done) 
  {
    GetEvent(&ourevent,0);
    //init = init +1;
  
    //ClearWorld(ourworld);

    // t = t + dt; //nächsten Zeitschritt berechnen
    initial();
    //drawgal(t, nring, v); // aufrufen der Zeichnenfunktion

    //delta_phi = delta_phi + 1; // für konstante Kreisfrequenz

    system ("sleep 0.01");

    if (ourevent.type == ButtonPress) //Event-Behandlung: Warten auf einen Mausklick.
    done =1;
  }
};

void initial()
{
  //Massen
  m_1 = 1.;
  m_2 = 1.;
  
  // Galaxie m_2
  r_2 = 3.; // Abstand von der x-Achse zur Galaxie
  ink_wk1 = -0.1 * M_PI; // Inklinationswinkel Zwischen der Bahnebene und der y-x-Ebene
  x_2 = -4;
  y_2 = cos(ink_wk1) * r_2;
  z_2 = sin(ink_wk1) * r_2;
  v_x2 = 0.1 ; // Anfangsgeschwindigk. v(t=0)
  v_y2 = 0.0 ;
  v_z2 = 0.0 ;
  
  // Galaxie m_1
  fac = - m_2 / m_1 ; // Massenverhältnis der Galaxien
  // Startwerte
  ink_wk2 = -0.6 * M_PI;
  x_1 = fac * x_2;
  y_1 = fac * y_2;
  z_1 = fac * z_2;
  v_x1 = fac * v_x2 ; 
  v_y1 = fac * v_y2 ;
  v_z1 = fac * v_z2 ;
  
  x[0] = x_1;
  x[1] = v_x1;
  x[2] = y_1;
  x[3] = v_y1;
  x[4] = z_1;
  x[5] = v_z1;
  x[6] = x_2;
  x[7] = v_x2;
  x[8] = y_2;
  x[9] = v_y2;
  x[10] = z_2;
  x[11] = v_z2;
  double dx, dy, dz, v_z, v_x, v_y, wphi, dphi, rp, vp, rstep = 1;
  // Festlegung der Galaxtischen Koordinaten der Galaxie 1
  int np = 2, npx, n0 ; // die zentralen Massen sind schon in x[]
  WFillCircle(ourworld, x[0], cos( phi) * x[2]+ sin( phi) * x[4], 1, 53); //Kreis zeichnen
  for ( int i = 1;  i <= nring ; i++ ) 
  {// Loop ueber Ringe
    rp = (i+1) * rstep ; // radialer Abstand von Zentralmasse
    vp = sqrt(m_1/rp) ; // Kreisbahngeschwindigkeit, G=1 (!)
    npx = i*6 ; // Anzahl der Teilchen auf aktuellem Ring
    dphi = 2.*M_PI/double(npx) ; // Winkelbruchteil
    for ( int n = 0 ; n < npx; n=n+1 ) // Loop ueber Teilchen auf dem Ring
    {
      wphi = double(n) * dphi ; // Winkel
      dx = rp * sin(wphi) ; // x relativ zur Zentralmasse
      dy = rp * cos(wphi) ;
      dz = rp * cos(wphi);
      v_x = vp * cos(wphi) ;	// v relativ zu v_Zentralmasse
      v_y = vp * (-1.*sin(wphi)) ;
      v_z = vp * (-1.*sin(wphi));
      n0 = 6 * (np + n) ; // Index berechnen
      x[n0] = x_1 + dx ; // absolute Koordinaten
      x[n0+1] = v_x1 + v_x ;
      x[n0+2] = y_1 + dy ;
      x[n0+1] = v_y1 + v_y ;
      x[n0+4] = z_1 ;
      x[n0+5] = v_z1 ;
      WFillCircle(ourworld, x[ n0], cos( phi) * x[ n0 + 2]+ sin( phi) * x[ n0 + 4], 0.5, 53); //Kreis zeichnen
    }
    np = np + npx ;
  }
  // Festlegung der Galaxtischen Koordinaten der Galaxie 2
  WFillCircle(ourworld, x[ 6], cos( phi) * x[ 8]+ sin( phi) * x[ 10], 1, 100); //Kreis zeichnen
  for ( int i = 1;  i <= nring ; i++ ) 
  {// Loop ueber Ringe
    rp = (i+1) * rstep ; // radialer Abstand von Zentralmasse
    vp = sqrt(m_2/rp) ; // Kreisbahngeschwindigkeit, G=1 (!)
    npx = i*6 ; // Anzahl der Teilchen auf aktuellem Ring
    dphi = 2.*M_PI/double(npx) ; // Winkelbruchteil
    for ( int n = 0 ; n < npx; n++ ) // Loop ueber Teilchen auf dem Ring
    {
      wphi = double(n) * dphi ; // Winkel
      dx = rp * sin(wphi) ; // x relativ zur Zentralmasse
      dy = rp * cos(wphi) * cos(ink_wk2);
      dz = rp * cos(wphi) * sin(ink_wk2);
      v_x = vp * cos(wphi) ;	// v relativ zu v_Zentralmasse
      v_y = vp * (-1.*sin(wphi)) * cos(ink_wk2) ;
      v_z = vp * (-1.*sin(wphi)) * sin(ink_wk2);
      n0 = 6 * (np + n) ; // Index berechnen
      x[n0] = x_2 + dx ; // absolute Koordinaten
      x[n0+1] = v_x2 + v_x ;
      x[n0+2] = y_2 + dy ;
      x[n0+1] = v_y2 + v_y ;
      x[n0+4] = z_2 ;
      x[n0+5] = v_z2 ;
      WFillCircle(ourworld, x[ n0], cos( phi) * x[ n0 + 2]+ sin( phi) * x[ n0 + 4], 0.5, 100); //Kreis zeichnen
    }
    np = np + npx ;
  }
};

double k;
int ndim;

void deriv( double x[], double k, int ndim, double m_1, double m_2)
{
  return ;
}
/*
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

;
*/




