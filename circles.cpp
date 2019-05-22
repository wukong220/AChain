#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <cstdlib>
using namespace std;

#define max(x, y)  ( x >= y? x : y )
#define min(x, y)  ( x <= y? x : y )
int number(double a, double b, double l, double phi);				//Number of circles with dencity of phi 
void now(ofstream* fout);
const double PI=3.1415926535897;	//pi

const int Type_circles = 1;				//atom type of circlestacles
const int MID_circles = 1;				//Mollecule ID of circles

const double L_box = 150.0;					//Lengh of the box

//chain setup
const double rad = 0.5;						//Radius of the chain
const double Sigma = 2 * rad;				//2 * radius of the chain * 2
const int N_chain = 30;						//Polarization of single chain
const int Num_chains = 1;					//Number of the chains

//circlestacle setup
const double rad_circles = 5.0;									//Radius of the circlestacle
const double R_circles = 2 * rad_circles ;							//2 * radius of the circlestacle * 2
const double Phi_circles = 0.58;									//Area density of the circlestacles
const double inter = Sigma + 2.5 * rad_circles;					//Minimal distace between beeds and circlestacles
const int Num_circles = number(rad_circles, rad_circles, L_box, Phi_circles);		//Number of the circlestacles

const int Num_atoms = Num_circles;			//Number of atoms
const int Num_bonds = 0;						//Number of bonds
const int Num_angles = 0;						//Number of angles
const int N_atomtypes = 1;						//Number of atom types
const int N_bondtypes = 0;						//Number of bond types
const int N_angltypes = 0;						//Number of angle types
	
struct Ranq2
{
	unsigned long long v, w;
	Ranq2(unsigned long long j) : v(4101842887655102017LL), w(1) {
		v ^= j;
		w = int64();
		v = int64();
	}
	inline unsigned long long int64() {
		v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
		w = 4294957665U * (w & 0xffffffff) + (w >> 32);
		return v ^ w;
	}
	inline double doub() { return 5.42101086242752217E-20 * int64(); }
	inline unsigned int int32() { return (unsigned int)int64(); }
};
struct Normaldev : Ranq2 {
	double mu, sig;
	Normaldev(double mmu, double ssig, unsigned long long i) : Ranq2(i), mu(mmu), sig(ssig) {}
	double dev();	//	Return a normal deviate.
};


int main()
{
	double dx;
	double dy;
	double dr;
	int t;

	Ranq2 Rand((unsigned int)(time(NULL)));
	
	// 2 dimensional array for particles
	double**  par = new double* [Num_circles];	
	for (int i = 0;i < Num_circles;i++)
			par[i] = new double[2];
	
	// Create Num_circles random particlar position with minimal distance inter
	for (int i = 0; i < Num_circles;)
	{
		par[i][0] = (L_box - rad_circles) * Rand.doub();
		par[i][1] = (L_box - rad_circles) * Rand.doub();
//		if(!checkin(L_box, inter, par, i))
			i++;
	}
	
	//Output file.data
	string filename = "circles.data";
	ofstream fout(filename);

	now(&fout);
	fout << Num_circles << " atoms\n\n\t\t"
	<< Num_bonds << " bonds\n\n\t\t"
	<< Num_angles <<" angles\n\n\t\t"
	<< N_atomtypes << " atom types\n\n\t\t"
	<< N_bondtypes << " bond types\n\n\t\t"
	<< N_angltypes << " angle types\n\n\t\t"
	<< 0 << " " << L_box << " xlo xhi\n\t\t"
	<< 0 << " " << L_box << " ylo yhi\n\t\t"
	<< -0.5 << " " << 0.5 << " zlo zhi\n\nMasses\n\n\t"
	<< Type_circles << "  " << 1.0 << "\n\nAtoms\n"
	<< endl;
		
	//Atoms
	for (int i = 0; i < Num_circles; i++) 
	{
		fout << i + 1 << "  "
		<< MID_circles << "  "
		<< Type_circles << "  "
		<< par[i][0] << "    "
		<< par[i][1] << "    "
		<< 0 << endl;

	}
	
	fout.close();
		
	for (int i=0; i < Num_circles; i++)
		delete[] par[i];
	delete[] par;
	
	return 0;
}



//	Return a normal deviate.
double Normaldev::dev()	
{
	double u, v, x, y, q;
	do {
		u = doub();
		v = 1.7156*(doub() - 0.5);
		x = u - 0.449871;
		y = abs(v) + 0.386595;
		q = x*x + y*(0.19600*y - 0.25472*x);
	} while (q > 0.27597 && (q > 0.27846 || v*v > -4.*log(u)*u*u));
	return mu + sig*v / u;
}

//Number of circlestacle with radius of r, with dencity of phi, in box of l
inline int number(double a, double b, double l, double phi)
{
	double s = PI * a * b;
	double S = l * l;
	double num = phi * S/s;
	return int(num);
}

//add time for data file
void now(ofstream* fout)
{
	time_t now = time(NULL);
	tm *ptm = localtime(&now);
	
	*fout << 1900 + ptm->tm_year << "/"
	<< 1 + ptm->tm_mon << "/"
	<< ptm->tm_mday << " "
	<< ptm->tm_hour << ":"
	<< ptm->tm_min
	<< " for LAMMPS data file from circles.cpp:" << "\n\n\t\t";
}