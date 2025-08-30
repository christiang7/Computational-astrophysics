#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <stdlib.h>     /* strtod */

using namespace ::std ;

double tau(double &w, double t[], int &N_0);
double ReadData( double x[], double t[]);
void FourierTrafo( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0);
void initial( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0);
void BuildSignal();
void median( double x[], double t[], int &N_0, double &x_med, double &t_med);


const long MaximalLaenge = 499999;

/* TO-DO-LISTE
 * Matrixgröße korrigieren
 * zum Schluss DOKU
 */


int main()
{
  int 
    N_0// Anzahl der Daten
  ;
  double 
    w,
    deltaw, // Schrittweite der Omegas Frequenzen
    ende // Intervallende der Frequenzen
    , x[MaximalLaenge] // Messwerte
    , t[MaximalLaenge] // Zeitwerte
    ;
  BuildSignal();
  initial( w, deltaw, ende, x, t, N_0);
  FourierTrafo( w, deltaw, ende, x, t, N_0);
}


void initial( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0)
{
  w = 0.;
  deltaw = 0.000001; // Schrittweite der Omega Frequenzen
  ende = 0.003; // Intervallende der Frequenzen
  N_0 = ReadData( x, t);
}

void BuildSignal()
{
  ofstream dateiout ; // Objekt der Klasse ofstream anlegen
  dateiout.open("TestDaten.dat") ; // Methode open der Klasse ofstream
  int i = 0 , v, vorzeichen;
  double deltat = 0.01, ampl = 0.75, freq = 5.,  ende = 10., x;
  double random, sigma = 0.1;
  time_t ttime ;    
  time(&ttime) ;
  srand((unsigned)time((long *) 0)) ;// initialize random number generator
  for (double t = 0. ; t <= ende ; t = t + deltat)
  {
    srand(i);
    random = rand()/ (RAND_MAX + 1.0);// Zufallszahlen zwischen 0 und 1
    x = random*(0.3);
    v = rand() % 2;
    if (v == 0)
    vorzeichen = +1.;
    else vorzeichen = -1.;
    dateiout << t << "	" << ( ampl * cos(freq * t) + 1 * vorzeichen * 1. /(sigma * sqrt(2.*M_PI) ) * exp(-(x *x)/(2. * sigma *sigma) ) )<< " " << 3. << endl ;
    //cout << x<< " " << vorzeichen * 1. /(sigma * sqrt(2.*M_PI) ) * exp(-(x * x)/(2. * sigma *sigma) ) << endl;
    i = i + 1;
  }
}

double ReadData( double x[], double t[])
{
  int k = 0;
  double g;
  char text[40], buffer[100];
  ifstream dateiin ; // Objekt der Klasse ifsteam anlegen
  dateiin.open("x-ray.dat") ;
  while ( !dateiin.eof() )
  {
    dateiin >> text;
    if (atof(text) == 0)
    {
	dateiin.getline(buffer,100) ;
    }
    if  (atof(text) != 0)
    {
	t[k] = strtod(text, NULL);
	dateiin >> x[k] >> g;
	//cout << t[k] <<" " << x[k] << " " << g << endl ;
	k = k + 1;
    }
  }
  dateiin.close() ;
  cout << (k - 1) << endl;
  return (k - 1);
}

void FourierTrafo( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0)
{ 
  double 
    P, alt_P = 0., big_w
    , d = 0., d2 = 0., f = 0., f2 = 0., var = 0.// Hilfsvariablen für die Summenberechnung
    , taw , x_med = 0., t_med = 0.; 
  ofstream dateiout ; // Objekt der Klasse ofstream anlegen
  dateiout.open("Fourier-x-ray.dat") ;
  median(x, t, N_0, x_med, t_med);
  for (w; w <= ende ; w = w + deltaw)
  {
    d = 0.;
    d2 = 0.; 
    f = 0.; 
    f2 = 0.;
    var = 0.;
    taw = tau(w, t, N_0);
    if (w != 0.)
    {
	for (int i = 0 ; i < N_0 ; i++)
	{
	  d = d + (x[i] - x_med) * cos(w * (t[i] - taw));//
	  d2 = d2 + cos(w * (t[i] - taw)) * cos(w * (t[i] - taw));
	  f = f + (x[i] - x_med) * sin(w * (t[i] - taw));//
	  f2 = f2 + pow(sin(w * (t[i] - taw)),2.);
	  var = var + (x[i] - x_med) * (x[i] - x_med);//
	}
	P = (1. / 2. * (d * d/ d2 + f * f / f2)) / (var / (double) N_0);
	if (P > alt_P) // Bestimmung der größten Frequenz
	{
	  big_w = w;
	  alt_P = P;
	}
    }
    else P = 0.;
    dateiout << w << " " << P << endl ;
  }
}

double tau(double &w, double t[], int &N_0)
{
  double
    si = 0.,
    co = 0.
    ;
  for (int i = 0 ; i < N_0 ; i++)
  {
    si = si + sin( 2. * w * t[i]);
    co = co + cos( 2. * w * t[i]);
  }
  return (atan(si / co) / ( 2. * w));
}

void median( double x[], double t[], int &N_0, double &x_med, double &t_med)
{
  int Min_x, Min_t , i ,j;
  double x_temp[N_0], t_temp[N_0], tausch1, tausch2, tausch;
  for (int k = 0; k < N_0; k++)
  {
    x_temp[k] = x[k];
    t_temp[k] = t[k];
  }
  for (j = 0; j < (N_0 - 1); j++) 
  {
    Min_x = j;
    Min_t = j;
    for (i = j + 1; i < N_0; i++) 
    {
      if ((x_temp[i] + 10000) < (x_temp[Min_x]+ 10000) )
	{
        Min_x = i;
      }
      if ((t_temp[i] + 10000) < (t_temp[Min_t]+ 10000) )
	{
        Min_t = i;
      }
    }
    if(Min_x != j) 
    {
	tausch1 = x_temp[Min_x];
	tausch2 = x_temp[j];
	tausch1 = tausch1 + tausch2;
	tausch2 = tausch1 - tausch2;
	tausch1 = tausch1 - tausch2;
	x_temp[Min_x] = tausch1;
	x_temp[j] = tausch2;
    }
    if(Min_t != j) 
    {
      t_temp[j] = t_temp[j] + t_temp[Min_t];
	t_temp[Min_t] = t_temp[j] - t_temp[Min_t];
	t_temp[j] = t_temp[j] - t_temp[Min_t];
    }
  }
  if (fmod(N_0, 2) == 0)
  {
    x_med = (double(x_temp[N_0 / 2]) + double(x_temp[(N_0) / 2 + 1])) / 2.;
    t_med = (double(t_temp[N_0 / 2]) + double(t_temp[(N_0) / 2 + 1])) / 2.;
  }
  else 
  {
    x_med = x_temp[(N_0) / 2 + 1];
    t_med = t_temp[(N_0) / 2 + 1];
  }
  /*for (int j = 0; j < (N_0); j++) 
  {
    cout << x_temp[j] << endl;
  }*/
  //cout << x_temp[N_0 / 2] << " " << x_temp[(N_0) / 2 + 1] <<  endl;
  //cout << t_med << " " << x_med  << endl;
}