#include <cmath>
#include "Xgraphics.h"
#include <iostream>
#include <time.h>//ctime
#include <stdlib.h>

using namespace ::std ;
//#define PI 3.14159265
//Zur Verwaltung der Fenster benotigte Variablen:

Window ourwindow;
World ourworld;
XEvent ourevent;


//srand((unsigned)time((long *) 0)) ; Zufallszahlengenerator Initialisierung erfolgt mittels der Funktion srand(unsigned)

const int nring = 10; // Anzahl der Ringe
const int lifetime = 15;
const int Anzahl_ring = int(6. * nring * (nring + 1.) / 2. + 1.); //Gesamtanzahl der vorhandenen Ringe
const int Vgrad = 360;
const int ninit = 300;

int
  cluster[nring][ninit+1] = { 0 }, // Array der Clusteraktivität
  uebergabewert = 0    
;

void srand(unsigned int startwert);
void drawgal (float t, int nring, float v);
void initial (int active_r [], int active_a [], int cluster[nring][ninit+1], int nactive);
void cluster_change (int cluster[nring][ninit+1], int nring, int uebergabewert);
void plotspiral (float &t, int cluster[] [ninit + 1],float v);
/*


void evolve (float &t, int active_a [], int active_r [], int cluster[][ninit + 1], int &nactive) ;
void create (int r, int a, int &newactive, int newactive_r [], int newactive_a [], int cluster[][ninit + 1]) ;
*/
//Hauptprogramm:
main() 
{  
  srand(static_cast<int>(time(NULL)));
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

  int 
    done = 0,
    nactive = 1,
    active_a[Anzahl_ring],
    active_r[Anzahl_ring]
  ;
  float 
    //a_0, Anfangsradius für die ersten 6 Kreise
    t = 0., //Zeitvariable
    dt = 0.001, //Zeitschritt
    v = 20. //Geschweindigkeit
  ;
  

// a_0 = r*(1.-sin(M_PI/n))/sin(M_PI/n);  Anfangsradius für die ersten 6 Kreise
  t = t + dt;
  initial(active_r, active_a, cluster, nactive);
  plotspiral(t, cluster, v);
  //cluster_change(cluster, nring, uebergabewert);
  while(!done) 
  {
    //init = init +1;
    //delta_phi = delta_phi + 1; // für konstante Kreisfrequenz
    GetEvent(&ourevent,0);
    
      if (ourevent.type == ButtonPress) //Event-Behandlung: Warten auf einen Mausklick.
      {
	uebergabewert = 0;
	//cluster_change(cluster, nring, uebergabewert);
	done = 1;
      }
      else
      {
	ClearWorld(ourworld);
	t = t + dt; // nächsten Zeitschritt berechnen
	//drawgal(t, nring, v); // aufrufen der Zeichnenfunktion
	plotspiral(t, cluster, v);
      }
    
    system ("sleep 0.01");
  }
  //Sauberes Beenden des Programms:
  ExitX();
  
  return 0;/* clean up memory */
}



void drawgal(float t, int nring, float v)
{
  float 
  r = 1.5,
  a,
   // phi, Winkelverschiebung/distanz
  // delta_phi = 0., 
  x ,y,
  w //Kreisfrequenz
  ;
  int n = 6;
  
  WFillCircle(ourworld, 0, 0, r, 50); //Mittelkreis zeichnen
  
  for(int l = 0; l < 2; ++l) //
  {
    for(int i = 0; i <= nring; ++i)
    {
      //N = n*i; Anzahl der Kreise auf dem i. Ring
      //phi = 360./N; Winkeldistanz von jedem einzelnem Ring
      a = r * (1. - sin(M_PI / (n * i))) / sin(M_PI / (n * i)); //Berechnung des neuen Abstands
      w = v / a; 
      /*if (init != 1)
      {
      delta_phi = (a_0 +r )/(a + r) + delta_phi; für konstante Kreisgeschwindigkeit
      //system ("sleep 0.001");
      }
      else delta_phi = (a_0 +r )/(a + r);*/
      for(int k = 0; k < (n * i); ++k)
      {
	x = (a + r) * cos(2. * k * M_PI / (n * i) + w * t);//Umrechnung von Polar in Kartesischen Koordianten- delta_phi
	y = (a + r) * sin(2. * k * M_PI / (n * i) + w * t);//- delta_phi
	if (fmod(k,i) == 0) // wenn k ein vielfaches von i ist dann färbe es blau
	{
	  WFillCircle(ourworld, x, y, r, 50); //Kreis zeichnen
	}
	else //ansonsten schwarz
	{
	  //cout << "x = " << x << " y = " << y << endl; // Koordiantenausgabe
	  WDrawCircle(ourworld, x, y, r, 1); //Kreis zeichnen
	}
      }
    };
  };//
  
};

void plotspiral(float &t, int cluster[] [ninit + 1], float v)
{
  float 
  r = 1.5,
  a,
   // phi, Winkelverschiebung/distanz
  // delta_phi = 0., 
  x ,y,
  w //Kreisfrequenz
  ;
  int n = 6;
  float faktor = 1;
  float radius;
  for(int l = 0; l < 2; ++l) //
  {
  for(int i = 0; i <= nring; ++i)
  {
    for(int k = 0; k < (n * i); ++k)
    {
      radius = float(cluster[i][k])/float(lifetime)*faktor;
      a = radius * (1. - sin(M_PI / (n * i))) / sin(M_PI / (n * i));
      w = v / a;
      x = (a + radius) * cos(2. * k * M_PI / (n * i) + w * t);//Umrechnung von Polar in Kartesischen Koordianten- delta_phi
      y = (a + radius) * sin(2. * k * M_PI / (n * i) + w * t);//- delta_phi
      if (cluster[i][k] > 0)
      WFillCircle(ourworld, x, y, radius, 50);
    }
  }
  };
};

void initial (int active_r [], int active_a [], int cluster[nring][ninit+1], int nactive)
{
  int winkel, abstand;
  for(int i = 0; i < Anzahl_ring; ++i)
  {
    abstand = rand()%nring;// + 1
    winkel = rand()%ninit;// + 1
    active_r[i] = abstand;
    active_a[i] = winkel;
    cluster[abstand][winkel] = lifetime;
    cout << "(" << abstand << "," << winkel <<") = " << cluster[abstand][winkel]<< endl;
  }
}
;

void cluster_change (int cluster[nring][ninit+1], int nring, int uebergabewert)
{
  for(int i = 0; i < nring; ++i)
  {
    for(int k = 0; k < (ninit); ++k)
    {
      // cluster[i][k] = uebergabewert;
      cout << "(" << i << "," << k <<") = " << cluster[i][k]<< endl; // Koodiantenausgabe
    }
  }
  
}
;