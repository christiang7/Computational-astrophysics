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
double ReadData( double x[], double t[], char Dateiname[]);
void FourierTrafo( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0, char Dateiname[]);
void initial( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0, char Dateiname[]);
void BuildSignal();
void median( double x[], int &N_0, double &x_med);


const long MaximalLaenge = 499999;

int main()
{
  int 
    N_0// Anzahl der Daten
  ;
  double 
    w, //Frequenz
    deltaw, // Schrittweite der Omegas Frequenzen
    ende // Intervallende der Frequenzen
    , x[MaximalLaenge] // Messwerte
    , t[MaximalLaenge] // Zeitwerte
    ;
  char Dateiname[200];
  BuildSignal(); // Test Signal erzeugen
  initial( w, deltaw, ende, x, t, N_0, Dateiname); // Anfangswerteerzeugung
  FourierTrafo( w, deltaw, ende, x, t, N_0, Dateiname);
}

void initial( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0, char Dateiname[])
{
  w = 0.;
  cout << "Geben sie den Dateinamen ein der sich im aktuellen Verzeichnis befindet:" << endl;
  cin >> Dateiname;
  cout << "Geben sie ein oberes Intervallende der Frequenzen ein:" << endl;
  cin >> ende; 
  N_0 = ReadData( x, t, Dateiname); // zur Bestimmung der Größe/Länge der Arrays
  deltaw = 1. / (t[N_0] - t[0]);
}

void BuildSignal()
{
  ofstream dateiout ; // Objekt der Klasse ofstream anlegen
  dateiout.open("TestDaten.dat") ; // öffnen der Datei
  int i = 0 , vorzeichen;
  double deltat = 0.01, ampl = 0.75, freq = 5.,  ende = 10., x; // Anfangswerte für das Test Signal
  double random, sigma = 0.1, T;
  time_t ttime ;    
  time(&ttime) ;
  srand((unsigned)time((long *) 0)) ;// initialisieren des Zufallsgenerators
  for (double t = 0. ; t <= ende ; t = t + deltat)
  {
    srand(i);
    random = rand()/ (RAND_MAX + 1.0);// Zufallszahlen zwischen 0 und 1
    x = random*(0.3);
    if (rand() % 2 == 0) //damit die Streuung nicht nur aufaddiert sondern auch mal substrahiert wird
    vorzeichen = +1.;
    else vorzeichen = -1.;
    dateiout << t << "	" << ( ampl * cos(freq * t) + 1 * vorzeichen * 1. /(sigma * sqrt(2.*M_PI) ) * exp(-(x *x)/(2. * sigma *sigma) ) )<< " " << 3. << endl ;
    i = i + 1;
  }
}

double ReadData( double x[], double t[], char Dateiname[])
{
  int k = 0;
  double g; // zum Einlesen der Sigmas die hier nicht weiter verwendet werden
  char text[40], buffer[100];
  ifstream dateiin ; // Objekt der Klasse ifsteam anlegen
  dateiin.open(Dateiname) ;
  while ( !dateiin.eof() ) // solange einlesen bis Dateiende erreicht
  {
    dateiin >> text;
    if (atof(text) == 0) // Überprüfung ob das erste Zeichen ein Zahl ist
    {
	dateiin.getline(buffer,100) ; // einlesen der ganzen Zeile damit bei der nächste Zeile angefangen wird
    }
    if  (atof(text) != 0)
    {
	t[k] = strtod(text, NULL); // Anfangs eingelesene Zahl in das Array packen
	dateiin >> x[k] >> g;
	//cout << t[k] <<" " << x[k] << " " << g << endl ;
	k = k + 1;
    }
  }
  dateiin.close() ;
  //cout << (k - 1) << endl;
  return (k - 1); // Leider geht die Schleife weiter als nötig und der zu viel gezählte Counter abgezogen werden
}

void FourierTrafo( double &w, double &deltaw, double &ende, double x[], double t[], int &N_0, char Dateiname[])
{ 
  double 
    P = 0., alt_P = 0., w_0 = 0.
    , co = 0., co2 = 0., si = 0., si2 = 0., var = 0.// Hilfsvariablen für die Summenberechnung
    , taw = 0. , x_med = 0.; 
  char Dateiout[208] = "Fourier-";
  ofstream dateiout ; // Objekt der Klasse ofstream anlegen
  for (int i = 0; i < 200 ; i = i + 1) // Namenserstellung der Outputdatei
  {
    Dateiout[i + 8] = Dateiname[i];
  }
  dateiout.open(Dateiout);
  median(x, N_0, x_med); // Median Bestimmung  
  for (double w; w <= ende ; w = w + deltaw)
  {
    co = 0.;
    co2 = 0.; 
    si = 0.; 
    si2 = 0.;
    var = 0.;
    taw = tau(w, t, N_0); // Bestimmung von Tau 
    if (w != 0.) // wenn der Sinus bei Null berechnet wird, wird bei der Berechnung von P durch Null geteilt
    {
	for (int i = 0 ; i < N_0 ; i++) // Summenberechnung der Fourieranalyse plus Varianz Bestimmung
	{
	  co = co + (x[i] - x_med) * cos(w * (t[i] - taw));//
	  co2 = co2 + cos(w * (t[i] - taw)) * cos(w * (t[i] - taw));
	  si = si + (x[i] - x_med) * sin(w * (t[i] - taw));//
	  si2 = si2 + pow(sin(w * (t[i] - taw)),2.);
	  var = var + (x[i] - x_med) * (x[i] - x_med);//
	}
	P = (1. / 2. * (co * co/ co2 + si * si / si2)) / (var / (double) N_0);
	if (P > alt_P) // Bestimmung der größten Frequenz
	{
	  w_0 = w;
	  alt_P = P;
	}
    }
    else P = 0.;
    dateiout << w << " " << P << endl ; // Ergebnisse in die offene Datei schreiben
  }
  dateiout << "# w_0 = " << w_0 << " P_0 = " << P << endl ; // Ergebnisse in die offene Datei schreiben
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

void median( double x[],int &N_0, double &x_med)
{
  int Min_x, i ,j;
  double x_temp[N_0];
  for (int k = 0; k < N_0; k++) // temporäries Feld zur Bestimmung des Medians
  {
    x_temp[k] = x[k];
    //cout << x_temp[k] << endl;
  }
  for (j = 0; j < (N_0 - 1); j++) // Sortierung des Arrays
  {
    Min_x = j; // Anfangsminimum setzen
    for (i = j + 1; i < N_0; i++) // Suche nach dem kleinsten Wert in dem verbleibenen Array
    {
      if ((x_temp[i]) < (x_temp[Min_x]) )
	{
        Min_x = i; // gefundenes Minimum speichern
      }
    }
    if(Min_x != j) // gefundenes Minimum mit der Einganszahl in der Schleife tauschen
    {
	x_temp[Min_x] = x_temp[Min_x] + x_temp[j];
	x_temp[j] = x_temp[Min_x] - x_temp[j];
	x_temp[Min_x] = x_temp[Min_x] - x_temp[j];
    }
  }
  if (fmod(N_0, 2) == 0) // Median Bestimmung wenn Länge des Arrays gerade ist
  {
    x_med = (double(x_temp[N_0 / 2]) + double(x_temp[(N_0) / 2 + 1])) / 2.;
  }
  else // für ungerade
  {
    x_med = x_temp[(N_0) / 2 + 1];
  }
  /*for (int j = 0; j < (N_0); j++) 
  {
    cout << x_temp[j] << endl;
  }*/
  //cout << x_temp[N_0 / 2] << " " << x_temp[(N_0) / 2 + 1] <<  endl;
  //cout << x_med  << endl;
}
