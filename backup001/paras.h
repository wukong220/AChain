#ifndef PARAS_H
#define PARAS_H

#include <cmath>
#include <vector>
#include <iostream>						//for test #include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef vector<vector<vector<double> > > vec_doub3;

extern ofstream output;
extern const int Num_info;				//id type xu yu zu...
extern const int dimension;
extern const double mass;
extern int Num_file;

extern const double md_dt;
extern int Num_frame;
extern int dNM;
//extern const int Max_frame;
extern const int framestep;

//input line by line
bool input(int &x);
bool input(string &str);
//time
string time (clock_t start);
vec_doub3 operator+=(vec_doub3 &vec0, const vec_doub3 vec1);
//output for check
ostream & operator<<(ostream & os, vector<int> &vec);
ostream & operator<<(ostream & os, vector<double> &vec);
ostream & operator<<(ostream & os, vector<string> &vec);
ostream & operator<<(ostream & os, vector<vector<int> > &vec);
ostream & operator<<(ostream & os, vector<vector<double> > &vec);
ostream & operator<<(ostream & os, vector<vector<string> > &vec);
//just for fun
ostream & operator<<(ostream & os, vector<vector<vector<double> > > &vec);	
//show information
vector<string> show(string &logname, string &finname, vector<string> &label, string &foutname, int &Num_chains, int &N_chain, int &Num_beeds, int &Max_frame);
//for test

#endif