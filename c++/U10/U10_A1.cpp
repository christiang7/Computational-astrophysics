#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <time.h>

using namespace ::std ;

int main ()
{
  // a.)
  double random, x, a =2.,b=10.;
  time_t ttime ;    
  time(&ttime) ;
  srand((unsigned)time((long *) 0)) ;// initialize random number generator
  random = rand()/ (RAND_MAX + 1.0);// Zufallszahlen zwischen 0 und 1
  x= random*(b-a); //Zufahlszahlen zwischen a und b
  cout << "random = " << x << endl ;
  random = rand()/ (RAND_MAX + 1.0);
  x= random*(1);
  cout << "random = " << x << endl ;
}