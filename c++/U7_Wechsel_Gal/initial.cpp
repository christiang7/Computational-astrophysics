#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

// intializiation of parameters for Kepler problem

using namespace std ;

int const np = 2 ;
int const nd = 2 ;

// String noetig um leere Zeilen bei der Eingabe zu erlauben
// Conversion String -> Double
template<class T> T fromString(const string& s)
{
     istringstream stream (s);
     T t;
     stream >> t;
     return t;
}
// input of value or set to default
template <class T> void definput(T &input, T defvalue)
{
  string line ;
  getline(cin, line) ;  
  if (line.empty()) input = defvalue ;
  else input = fromString<T>(line) ;      
} 

//------------------------------------------------------------------------------------

void initial (double pos[np][nd], double vel[np][nd], double &t, double &GM, 
              double &dt, int &nplot, double ratio[np]) 
{

  double velstart, pos1 ;
  GM = 4.0 * M_PI * M_PI ; // natural units ; note that
  //  1 length unit = 1.496 x 10^11 m
  //  1 time unit   = 3.156 x 10^7 s
  //  1 vel unit    = 4.740 km/s 
  
  t  = 0. ;      // initial time

  cout << "Eingabe der Zeitschrittweite [1.E-5]: " ;
  definput<double>(dt, 1.E-5) ; // time step width

  cout << "Anzahl der Zeitschritte pro Plot [10000]: " ;
  definput<int>(nplot, 10000) ; // make output every nplot timestep

  // planet 1  
  pos1 = 2.5191292 ;
  cout << "Planet 1 Abstand von der Sonne in AU ["<<pos1<<"]: " ; 
  definput<double>(pos[0][0], pos1) ;  // initial x-position
  pos[0][1] = 0. ;  // initial y-position
  vel[0][0] = 0. ;   // initial x-velocity     
  velstart =  sqrt(GM / fabs(pos[0][0]) ) ;
  cout << "Planet 1 Anfangsgeschwindigkeit [" << velstart << "]: " ;
  definput<double>(vel[0][1], velstart) ; // initial y-velocity
  ratio[0] = 0.04 * GM ; // mass of planet 2 acting on planet 1

  // planet 2
  cout << "Planet 2 Abstand von der Sonne in AU [5.24]: " ; 
  definput<double>(pos[1][0], 5.24) ;  // initial x-position
  pos[1][1] = 0. ;  // initial y-position
  vel[1][0] = 0. ;   // initial x-velocity     
  velstart =  sqrt(GM / fabs(pos[1][0]) ) ;
  cout << "Planet 2 Anfangsgeschwindigkeit [" << velstart << "]: " ;
  definput<double>(vel[1][1], velstart) ; // initial y-velocity
  ratio[1] = -0.001 * GM ; // mass of planet 1 acting on planet 2

}

//------------------------------------------------------------------------------------
