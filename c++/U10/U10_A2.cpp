#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "Xgraphics.h"

using namespace ::std ;

Window ourwindow;
World ourworld;
XEvent ourevent;

void initial(int &n, double &p_a, double &dicke, double &lambda);//double &p_s, 
double scatter( double &x, double &lambda, int &j);
//void output();

int main ()
{
  //b.)
  InitX(); /* connect to X-server */
  
  /* set up windows and worlds */
  ourwindow = CreateWindow(800,800,(char*)"demo1");
  ourworld = CreateWorld(ourwindow,0,0,800,800,-20,40,60,-40,0,0);
  /* width, height
  /* name of window
  /*
  window to draw in
  x,y,width,height
  wx1, wy1, wx2, wy2
  scaling property
  "gravity"
  */
  ShowWindow(ourwindow);
  DrawLine (ourwindow, -20, -40, -20, 60, 2);
  // a.)
  int 
	n, // Anzahl der Neutronen
	absorb = 0, //Anzahl der absorb. Neutronen
	scat = 0, // Anzahl gestreuter Neutronen
	j = 0 // Zählt die Zufahlszahlen durch
	;
  double 
	p_a,  //p_s,Wahrscheinlichkeiten 
	dicke,// Dicke
	random1,
	random2,
	lambda, 
	x = 0.
	;
  int done = 0;
  while(!done) 
  {
    GetEvent(&ourevent,0);
  
    //ClearWorld(ourworld);
  initial( n, p_a, dicke, lambda);//p_s, 
  for (int i = 1 ; i <= n ; i++)
  {
	x = 0;
	//system ("sleep 1") ;
	time_t ttime ;
	time(&ttime) ;
	srand((unsigned)time((long *) 0)) ;// initialize random number generator
	srand(i);
	random1 = rand()/ (RAND_MAX + 1.0);// Zufallszahlen zwischen 0 und 1
	if (random1 <= p_a)
	{
	  absorb = absorb + 1;
	  cout << "abs Neut " << random1 <<" " << absorb << " bei " << x << endl ;
	}
	else
	{
	  //random2 = random1;
	  while( (x <= dicke) && (random1 > p_a) && (x >= 0) )
	  {
		
		j = j + i + 1;
		//system ("sleep 1") ;
		x = scatter( x, lambda, j);
		//system ("sleep 1") ;
		//srand((unsigned)time((long *) 0));
		srand(j);
		if ((x <= dicke) && (x >= 0))
		{
		cout << "noch drin " << random1 << " bei " << x << endl ;
		random1 = rand()/ (RAND_MAX + 1.0);
		}
	  }
	  if((random1 <= p_a) &&  (x <= dicke) && (x >= 0) )//
	  {
	    absorb = absorb + 1;
	    cout << "abs Neut " << random1 <<" " << absorb <<" bei " << x <<  endl ;
	  }
	  else
	  {
	    scat = scat +1; // zähle die durchkommenden Neutronen
	    cout << "scat Neut " << random1 << " " << scat << " bis " << x <<   endl ;
	  }
	}
  };
  cout << "absorbierte Neutronen = " << absorb << endl ;
  cout << "gestreute Neutronen = " << scat << endl ;
  system ("sleep 0.01");
  
  if (ourevent.type == ButtonPress) //Event-Behandlung: Warten auf einen Mausklick.
  done =1;
}
  
};

void initial(int &n, double &p_a, double &dicke, double &lambda)//double &p_s, 
{
  double 
    N = 100., // Anzahl der Teilchen im Stoff
    R = 0.5 // Radius der Teilchen im Stoff 
    ;
  n = 100;
  p_a = 0.2; // Wkeit für Absorption
  double p_s = 1. - p_a; // Wkeit für Streuung
  dicke = 6.;
  lambda = pow(dicke,3) / (N * R * R * M_PI); // mittlere freie Weglänge
  cout << "Anfangswerte " << endl ;
  cout << "Anzahl Neutronen = " << n << endl ;
  cout << "Wkeit absorb. Neutronen = " << p_a << endl ;
  cout << "Wkeit gestreute Neutronen = " << p_s << endl ;
  cout << "mittlere freie Weglänge = " << lambda << endl ;
};

double scatter(double &x, double &lambda, int &j)
{
  double 
    random, // Zufahlszahlen
    cotheta, // Streuwinkel
    l //, zurückgelegter Weg nach Streuung
    ;
  //time_t ttime ;
  //time(&ttime) ;
  //system ("sleep 1") ;
  //srand((unsigned)time((long *) 0)) ;
  srand(j+1);
  random = rand()/ (RAND_MAX + 1.0);
  cotheta = 1. - 2.*random; // cos(theta) Winkelberechnung bei der Ablenkung
  //time(&ttime) ;
  //system ("sleep 1") ;
  //srand((unsigned)time((long *) 0)) ;
  srand(j+2);
  random = rand()/ (RAND_MAX + 1.0);
  l = - ( lambda ) * log(random); // Längenberechnung bei der der Ablenkung 
  return x + l * cotheta; // Verschiebung der gestreuten Neutronen
}
;
