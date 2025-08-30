#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

// intializiation of parameters for Kepler problem

using namespace std ;

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

void initial (float pos[], float vel[], float &t, float &GM, float &dt, int &nplot) 
{

  float velstart ;
  GM = 4.0 * M_PI * M_PI ; // natural units ; note that
  //  1 length unit = 1.496 x 10^11 m
  //  1 time unit   = 3.156 x 10^7 s
  //  1 vel unit    = 4.740 km/s 
  
  t  = 0. ;      // initial time

  cout << "Eingabe der Zeitschrittweite [0.001]: " ;
  definput<float>(dt, 0.001) ; // time step width

  cout << "Anzahl der Zeitschritte pro Plot [5]: " ;
  definput<int>(nplot, 5) ; // make output every nplot timestep

  cout << "Abstand von der Sonne in AU [1.]: " ; 
  definput<float>(pos[0], 1.) ;  // initial x-position

  pos[1] = 0. ;  // initial y-position
  vel[0] = 0 ;   // initial x-velocity

  velstart =  sqrt(GM / pos[0] ) ;
  cout << "Anfangsgeschwindigkeit [" << velstart << "]: " ;
  definput<float>(vel[1], velstart) ; // initial y-velocity

}

//------------------------------------------------------------------------------------
