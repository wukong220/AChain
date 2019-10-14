#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
typedef vector<vector<vector<double> > > vec_doub3;

class LmpFile
{
private:
	//int m_NumFile;
	vector<int> m_files;	//cnt[NumFile, EffFile, cnt]	
	string m_head;
	vector<vector<string> > m_fnamebel;	//filename[ifile][name,label]
	vector<vector<int> > m_frames;		//frames[ifile][Num_frame, Max_frame]
	
public:
	//constructors and destructors
	LmpFile();
	LmpFile(const vector<string>);
	LmpFile(const vector<vector<string> >, const int);
	LmpFile(const vector<vector<string> >, const int, const int);
	LmpFile(const vector<vector<string> >, const int, const vector<vector<int> >);
	~LmpFile();
	friend ostream & operator<<(ostream & os, const LmpFile & file);
	int files(){return m_files[1];};
	
	//ifile, atom, center
	vec_doub3 read_data(const int ifile, ofstream &output, const int nAtoms, const vector<int> closefiles = { });	//ifile, closefile[i]
	
	//ifile, N_chain, atom[ifile][jatom][xu, yu, zu...]
	vec_doub3 center(const int ifile, const int nChain, const vec_doub3 vec, const int d = 2);	//ifile, N_chain, atom
	//ifile, rCM[ifile][jchain][x, y, z], count[ifile][dframe], msd[iframe][jchain][0, x, y, z]
	vec_doub3 msd_point(const int ifile, const vec_doub3 vec, vec_doub3 &msd);	
	vec_doub3 msd(const int ifile, const vec_doub3 vec, vec_doub3 &msd, vec_doub3 &msd_com, const string label = "all");
	
	//continue
	void out_msd(const string foutname, const vec_doub3 vec_com, const vec_doub3 vec_ave, const vector<string> &label = {"all", "short"});
};

string read_atoms(ifstream &, int, int, vec_doub3 &); 	// fin, iframe, nAtoms, atom[i][j][k]

#endif