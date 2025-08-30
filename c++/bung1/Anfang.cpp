#include <iostream>
#include <cmath>
using namespace std ;

//float zehnerpotenz(float arg);

int main ()
{
	float T_eff, L_in, T_sonne = 5778, R_stern; // R_stern wird in Sonnenradien berechnet
	cout << "Geben sie eine effektive Temperatur T in Kelvin ein:" << endl;
	cin >> T_eff;
	while (T_eff < 0)
	{
		cout << "Falsche Eingabe, bitte geben sie eine positive effektive Temperatur T in Kelvin ein:" << endl;
		cin >> T_eff;
	}
	cout << "Geben sie eine Leuchtkraft L in logL/L_sonne ein:" << endl;
	cin >> L_in;
	R_stern = sqrt(pow(T_eff/T_sonne,4)*pow(L_in,10));
	cout << "Ergebnis: R_stern/R_sonne="<< R_stern << endl;
}
/*
float potenz(float arg)
{
 return arg*arg*arg*arg*arg*arg*arg*arg*arg*arg;
}

*/
