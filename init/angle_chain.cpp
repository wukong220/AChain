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
bool checkin(double l, double rm, double** pp, int n);
void now(ofstream* fout);

const double PI=3.1415926535897;	//pi
const int Type_chain = 1;			//atom type of chains
const int MID_chain = 1;			//Mollecule ID of chains

const int N_atomtypes = 2;		//Number of atom types
const int N_bondtypes = 1;		//Number of bond types
const int N_angltypes = 1;		//Number of angle types

const double L_box = 150.0;					//Lengh of the box
const double n_box = 3;	

//chain setup
const double rad = 0.5;						//Radius of the chain
const double Sigma = 2 * rad;				//2 * radius of the chain * 2
const int N_chain = 30;						//Polarization of single chain
const int Num_chains = 1;					//Number of the chains

const int Num_beeds = N_chain * Num_chains; 			//Number of beeds
const int Num_atoms = N_chain * Num_chains;				//Number of atoms
const int Num_bonds = Num_chains * (N_chain-1);			//Number of bonds
const int Num_angles = Num_chains * (N_chain - 2);		//Number of angles
	
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
	int t;

	Ranq2 Rand((unsigned int)(time(NULL)));
	
	double** chain = new double* [Num_beeds];
	for (int i = 0; i < Num_beeds; i++)
			chain[i] = new double[2];
			
	int k;
	double l = L_box * n_box;
	double dx = 0;
	double dy = 0;
	double dx0 = 0;
	double dy0 = 0;
	double dr = 0;
	for (int i = 0; i < Num_chains; i++)
		{
			chain[i][0] = l/2.0;	//chain[i][0] = L_box * Rand.doub() - 0.5 * L_box;
			chain[i][1] = l/2.0;	//chain[i][1] = L_box * Rand.doub() - 0.5 * L_box;
			for (int j=0; j < N_chain - 1; j++)
				{
					k = i * N_chain + j;
					dx0 = Rand.doub() * Sigma;
					dy0 = sqrt(Sigma * Sigma - dx0 * dx0);
					chain[k + 1][0] = chain[k][0] + dx0;
					chain[k + 1][1] = chain[k][1] - dy0;
	//				chain[k + 1][1] = chain[k][1] + Rand.doub() * Sigma; 
					if (k == 0)
						continue;
					dx = chain[k + 1][0] - chain[k - 1][0];
					dy = chain[k + 1][1] - chain[k - 1][1];
					dr = sqrt(dx * dx + dy * dy);
					if (dr < Sigma)
						chain[k + 1][1] = chain[k][1] - sqrt(Sigma * Sigma - dx * dx);
				}
		}
		
	//Output file.data
	string filename = "chain.data";
	ofstream fout(filename);

	now(&fout);
	fout << Num_atoms << " atoms\n\n\t\t"
	<< Num_bonds << " bonds\n\n\t\t"
	<< Num_angles <<" angles\n\n\t\t"
	<< N_atomtypes << " atom types\n\n\t\t"
	<< N_bondtypes << " bond types\n\n\t\t"
	<< N_angltypes << " angle types\n\n\t\t"
	<< 0 << " " << L_box << " xlo xhi\t#L_box 0 " << L_box <<"\n\t\t"
	<< 0 << " " << L_box << " ylo yhi\t#L_box 0 " << L_box <<"\n\t\t"
	<< -0.5 << "  " << 0.5 << " zlo zhi\n\nMasses\n\n"
	<< Type_chain << "  " << 1.0 << endl
	<< 2 << "  " << 1.0 << "\n\nAtoms\n"
	<< endl;
		
	//Atoms
	for (int i = 0; i < Num_beeds; i++)
	{
		fout << i + 1 << "  "
		<< Type_chain << "  "
		<< MID_chain << "  "
		<< chain[i][0] << "  "
		<< chain[i][1] << "  "
		<< 0 << endl;
	}
		
	//Bonds
	fout << "\nBonds\n\n";
	for (int i = 0; i < Num_chains; i++)
	{
		for(int j = 0; j < N_chain - 1; j++)
		{
			int k = (i * N_chain) + j;
			fout << k + 1 << "  " 
			<< Type_chain << "  " << k + 1 << "  " << k + 2 << endl;
		}
	}
	
	//Angles
	fout<< "\nAngles\n\n";
	for(int i = 0; i < Num_chains; i++)
	{
		for(int j=0; j < N_chain - 2; j++)
		{
			int k = (i * N_chain) + j;
			fout << k + 1 << "  " 
			<< Type_chain << "  " << k + 1 << "  " << k + 2 << "  " << k + 3 << endl;
		}	
	}
	
	fout.close();
	
	for (int i=0; i < Num_beeds; i++)
			delete[] chain[i];
		delete[] chain;
	
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

//check nth particle with other particles within distace rm
inline bool checkin(double l, double rm, double** pp, int n)
{
	int t = 0;
	double dx;
	double dy;
	double dr;
	for(int i = 0; i < n; i++)
	{
		//period boundary
		dx = abs(pp[n][0] - pp[i][0]);
		dx = min(dx, l - dx);
		dy = abs(pp[n][1] - pp[i][1]);
		dy = min(dy, l - dy);
		dr = dx * dx + dy * dy;
		if(dr > (rm * rm)) 
			t++;
		else
			return true;
	}
	if (t == n ) 
		return false; 
	else 
		return true;
}

//Number of obstacle with radius of r, with dencity of phi, in box of l
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
	<< " for LAMMPS data file from angle_chain.cpp:" << "\n\n\t\t";
}