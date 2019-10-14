#include "files.h"
#include "paras.h"


//extern const int Num_file, Num_frame
LmpFile::LmpFile()
{
	stringstream ss, sl;
	m_files = vector<int> {Num_file, Num_file};		//m_files[Num_file, files, count]
	//m_files.insert(m_files.end(), count, 0);	//for count
	m_head = "none";
	m_fnamebel = vector<vector<string> > (m_files[0], vector<string>(2));
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
	for (int i = 0; i < m_files[0]; i++)
	{
		if (i < 9)
		{
			ss << "00" << i + 1 << "u.lammpstrj";
			sl << "00" << i + 1;
		}
		else if (i >= 9 && i < 100)
		{
			ss << "0" << i + 1 << "u.lammpstrj";
			sl << "0" << i + 1;
		}
		ss >> m_fnamebel[i][0];
		sl >> m_fnamebel[i][1];
		ss.clear();
		sl.clear();
	}
}

LmpFile::LmpFile(const vector<string> finname)
{
	stringstream ss, sl;
	int num = finname.size();
	m_files = vector<int>{num, num};
	//m_files.insert(m_files.end(), count, 0);	//for count
	m_head = "none";
	m_fnamebel = vector<vector<string> > (m_files[0], vector<string>(2));
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
	
	if (finname[0] == "\0")
	{
		for (int i = 0; i < m_files[0]; i++)
		{
			if (i < 9)
			{
				ss << "00" << i + 1 << "u.lammpstrj";
				sl << "00" << i + 1;
			}
			else if (i >= 9 && i < 100)
			{
				ss << "0" << i + 1 << "u.lammpstrj";
				sl << "0" << i + 1;
			}
			ss >> m_fnamebel[i][0];
			sl >> m_fnamebel[i][1];
			ss.clear();
			sl.clear();
		}
	}
	else
	{
		for (int i = 0; i < m_files[0]; i++)
		{
			m_fnamebel[i][0] = finname[i] + "u.lammpstrj";
			if (i < 9)
				ss << "00" << i + 1;
			else if (i >= 9 && i < 100)
				ss << "0" << i + 1;
			ss >> m_fnamebel[i][1];
			ss.clear();
		}
	}

}

LmpFile::LmpFile(const vector<vector<string> > fnamebel, const int count)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num};
	m_files.insert(m_files.end(), count, 0);	//for count
	m_head = "none";
	m_fnamebel = fnamebel;
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
}

LmpFile::LmpFile(const vector<vector<string> > fnamebel, const int count, const int a)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num};
	m_files.insert(m_files.end(), count, 0);	//for count
	m_head = "none";
	m_fnamebel = fnamebel;
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{a, a - dNM});
}

LmpFile::LmpFile(const vector<vector<string> > fnamebel, const int count, const vector<vector<int> > frames)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num};
	m_files.insert(m_files.end(), count, 0);	//for count
	m_head = "none";
	m_fnamebel = fnamebel;
	m_fnamebel.insert(m_fnamebel.begin(), m_fnamebel[0]);
	m_frames = frames;
	m_frames.insert(m_frames.begin(), m_frames[0]);
}

LmpFile::~LmpFile()
{
	
}

ostream & operator<<(ostream & os, const LmpFile & file)
{
	//os.setf(ios::adjustfield);
	os << "Information: \n";
	for(int i = 0; i < file.m_head.size(); i++)
	{
		if ((file.m_head[i] >= 'A' && file.m_head[i] <= 'Z')|| file.m_head[i] == ':')
			continue;
		else
			os << file.m_head[i];
	}
	os << "\n\n";
	
	os << "Effective Files: " << file.m_files[1] << endl;
	os << setw(25) << left << "Name ";
	os << setw(5) << "Label";
	os << " ";
	os << setw(7) << left << "Num";
	os << " ";
	os << setw(7) << left << "Max" << endl;
	for (int i = 0; i < file.m_files[0]; i++)
	{
		os << setw(25) << left << file.m_fnamebel[i][0] << " ";
		os << setw(5) << file.m_fnamebel[i][1];
		for (int k = 0; k < file.m_frames[i + 1].size(); k++)
			os << setw(7) << left << file.m_frames[i + 1][k] << " ";
		os << endl;
	}
	os << endl;
	return os;
}

vec_doub3 LmpFile::read_data(const int ifile, ofstream &output, const int nAtoms, const vector<int> closefiles)
{
	//input	
	//extern const int Num_info = 15;
	vec_doub3 atom(m_frames[0][0], vector<vector<double> >(nAtoms, vector<double>(Num_info,0)));
	string error = "Right";
	int timestep = 0;
	string temp;
	stringstream ss;
	int Num_atoms = 1;
	
	ifstream fin(m_fnamebel[ifile][0]);
	if (m_fnamebel[0][1] == "000")
		m_files[1] = 1;
	
	for (int j = 0; j < closefiles.size(); j++)
	{
		if (ifile == closefiles[j] - 1)
			error = "\"CLOSE\": ";
	}
	
	if(!fin.is_open())
		error = "\"ERROR\": Cannot open ";
	
	//error information
	if(error != "Right")
	{
		cout << error << m_fnamebel[ifile][0] << endl;
		output << error << m_fnamebel[ifile][0] << endl;		//for output
		
		m_files[1]--;
		m_fnamebel[ifile][1] = "  ";
		
		m_frames[ifile + 1][0] = 0;
		m_frames[ifile + 1][1] = 0;
		
		error = "Right";
		return atom;
	}
	else
	{
		cout << "\"Opening\": " << m_fnamebel[ifile][0] << "……" << endl;
		output << "\"Opening\": " << m_fnamebel[ifile][0] << "……" << endl;
	}
	
	for (int i = 0; i < m_frames[0][0]; i++)
	{
		for (int clear = 0; clear < 2; clear++)			//the head
			getline(fin, temp);
		ss << temp;
		ss >> timestep;
		if (timestep != i * framestep)		//extern const int framestep = 5000;
		{
			error = "\"ERROR\": TIMESTEP/FRAME(files.cpp:198) -> ";
			cout << error << timestep << " != " << i * framestep << " (" << i << " * " << framestep << ")" << endl;
			output << error << timestep << " != " << i * framestep << " (" << i << " * " << framestep << ")" << endl;
			m_files[1]--;	
			m_fnamebel[ifile][1]= "000";
			m_frames[ifile + 1][0] = i - 1 ;					//frames[Num_frames, max_frames]
			m_frames[ifile + 1][1] = m_frames[ifile + 1][0] - dNM;
			break;
		}
		ss.clear();
		m_head = read_atoms(fin, i, nAtoms, atom);
		//read_type(fin, i, type, atom);
	}
	fin.close();
	//extern const int dNM = 3000;
	//m_frames[ifile + 1][1] = m_frames[ifile + 1][0] - dNM;	//frames[Num_frames, max_frames]
	if (m_files[1] == 0 && error == "Right")
		m_files[1] = 1;								//single file	
	return atom;
}

//average of every nChain vector: d+1 of vec -> position of atoms
vec_doub3 LmpFile::center(const int ifile, const int nChain, const vec_doub3 vec, int d)
{
	//extern int dimension = 2
	//extern double mass = 1.0
	int NumChains = vec[0].size() / nChain;
	int frames = vec.size();
	double m = mass;		//for mass
	if (d == 0)				//for test of ave
		m = 1;
	vec_doub3 rCM(frames, vector<vector<double> >(NumChains, vector<double>(d + 1, 0)));
	for (int i = 0; i < frames; i++)			
	{
		for (int j = 0; j < NumChains; j++)
		{
			for (int k = nChain * j; k < nChain * (j + 1); k++)
			{	
				for (int dim = 0; dim < d + 1; dim++)
					rCM[i][j][dim] += vec[i][k][dim] * m; //rCM[iframe][jchain][0, x, y, z]
			}
			double TotMass = nChain * m;
			for (int dim = 0; dim < d + 1; dim++)
				rCM[i][j][dim] /= TotMass;
		}
	}
	return rCM;
}

vec_doub3 LmpFile::msd_point(const int ifile, const vec_doub3 vec, vec_doub3 &msd)
{
	//extern int Max_frame; 
	//Num_chains
	int num = vec[0].size();	//Num_chains or Num_beeds
	//m_files.insert(m_files.begin()+2, num, 0);
	vector<vector<int> > cnt(m_frames[0][1], vector<int>(num, 0));
	for (int dt = 1; dt <= m_frames[ifile + 1][1]; dt++)
	{
		for(int Tstart = 0; Tstart < min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt); Tstart++)
			{
				int Tstop = Tstart + dt;
				for(int i = 0; i < num; i++)
				{
					//extern int dimension = 2
					int j = (dimension + 1) * ifile + 1;
					//count[ifile+1][dt-1]++;
					cnt[dt-1][i]++;
					msd[dt-1][i][j+dimension] = 0;		//for different files
					for (int dim = 0; dim < dimension; dim++)
					{
						msd[dt-1][i][j+dim] += (vec[Tstop][i][dim] - vec[Tstart][i][dim])*(vec[Tstop][i][dim] - vec[Tstart][i][dim]); 
						msd[dt-1][i][j+dimension] += msd[dt-1][i][j+dim];
					}
					
					if (m_fnamebel[ifile][1] != "000")
					{
						if (Tstart == (min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt)-1))
						{
							if (m_frames[ifile + 1][1] == (m_frames[0][1]) && m_frames[ifile + 1][0] == m_frames[0][0])		//effective files
							{	
								for (int dim = 0; dim <= dimension; dim++)
									msd[dt-1][i][j+dim] /= cnt[dt-1][i]; 
								//msd[dt-1][i][0] = (msd[dt-1][i][0] * (m_files[i+2]) + msd[dt-1][i][j+2]) / (m_files[i+2] + 1);
								msd[dt-1][i][0] += msd[dt-1][i][j+dimension];
							}
							//if (dt == m_frames[0][1])
							//	m_files[i+2]++;
						}
					}
					else if (m_fnamebel[ifile][1] != "  " && Tstart == (min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt)-1))
					{
						for (int dim = 0; dim <= dimension; dim++)
							msd[dt-1][i][j+dim] /= cnt[dt-1][i]; 
						//msd[dt-1][i][j+2] = msd[dt-1][i][j] + msd[dt-1][i][j+1];
					}
				}
			}
		if (ifile == Num_file - 1)
		{
			for(int i = 0; i < num; i++)
			{
				//m_files[i+2] = 0;		//initialization of count
				msd[dt-1][i][0] /= m_files[1];
			}
		}
	}
	return msd;
}

vec_doub3 LmpFile::msd(const int ifile, const vec_doub3 vec, vec_doub3 &msd, vec_doub3 &msd_com, const string label) //, vec_doub3 &msd_ave
{
	string error = "Right";
	int Num_frame = vec.size();
	if (Num_frame != m_frames[0][0])
		error = "Wrong Num_frame!";
	int NumBeeds = vec[0].size();
	int Max_frame = msd_com.size();
	if (Max_frame != m_frames[0][1])
		error = "Wrong Max_frame!";
	int NumChains = msd_com[0].size();
	int nChain = NumBeeds / NumChains;
	//int nChain = msd_ave[0][0].size() - 1;
	//if (NumBeeds != NumChains * nChain)
	//	error = "Wrong Atoms!";
	if (error != "Right")
	{
		cout << error << endl;
		exit(1);
	}
	//vec[iframe] [id] [id,type,xu,yu,zu...]
	//rCM[iframe] [jchain] [x,y,z]
	vec_doub3 rCM(m_frames[0][0], vector<vector<double> >(NumChains, vector<double>(dimension + 1,0)));
	vec_doub3 msd_ave(m_frames[0][0], vector<vector<double> >(NumChains, vector<double>(nChain + 1, 0))); 
	//rAtom[iframe] [jatom] [x, y, z]
	vec_doub3 rAtom(m_frames[0][0], vector<vector<double> >(NumBeeds, vector<double> (3, 0)));
	for (int i = 0; i < m_frames[0][0]; i++)
		for (int j = 0; j< NumBeeds; j++)
			for (int dim = 0; dim < 3; dim++)
				rAtom[i][j][dim] = vec[i][j][dim+2];
	//msd[dt] [iatom] [0, x1,y1,z1,r1, x2,y2,z2,r2...]: 0 for average of Num files, (dimension+1)for sum
	
	if (label == "com")
	{
		rCM = center(ifile, nChain, rAtom);
		//msd_com[iframe] [jchain] [0,x,y,z,r]: 0 for average of Num files, (dimension+1)for sum
		msd_point(ifile, rCM, msd_com);
		//return msd_com;
	}
	else if (label == "ave")
	{
		//msd[dt] [iatom] [0, x1,y1,z1,r1, x2,y2,z2,r2...]: 0 for average of Num files, (dimension+1)for sum
		msd_point(ifile, rAtom, msd);
		//if(ifile == m_files[0] - 1;
		for (int dt = 1; dt <= m_frames[0][1]; dt++)
		{
			for (int i = 0; i < NumChains; i++)
			{
				//m_fnamebel[ifile][1] != "000" "  "
				//msd_ave[dt-1][i][0] = 0; 
				for (int j = 0; j < nChain; j++)
				{
					int k = i * nChain + j;
					//msd_ave[dt] [ichain] [0, r1,r2,r3 ...]: 0 for average of N beeds
					//if (m_fnamebel[ifile][1] != "  ")
						msd_ave[dt-1][i][j+1] = msd[dt-1][k][0];
					//if (m_fnamebel[ifile][1] != "000")
						msd_ave[dt-1][i][0] += msd_ave[dt-1][i][j+1];
				}
				//if (m_fnamebel[ifile][1] != "000" && m_fnamebel[ifile][1] != "  ")
					msd_ave[dt-1][i][0] /= nChain;
			}
		}
		//for test[iframe] [jchain] [0]
		/*vec_doub3 test(m_frames[0][0], vector<vector<double> >(NumChains, vector<double>(1, 0)));
		test = center(ifile, nChain, msd, 0);	 //msd_ave[dt-1][i][0];		
		for (int i = 0; i < m_frames[ifile + 1][1]; i++)
		{
			for (int j = 0; j < NumChains; j++)
				//cout << "test msd:\n" << test[i][j][0] << " " << msd_ave[i][j][0] << endl;
				msd_ave[i][j][0] = test[i][j][0];
		}*/
		//return msd_ave;
	}
	else if (label == "all" || label == "\0")
	{	
		//msd_point(ifile, rAtom, msd);
		rCM = center(ifile, nChain, rAtom);
		//msd_com[iframe][jchain][0,x,y,z,r]: 0 for average of Num files, (dimension+1)for sum
		msd_point(ifile, rCM, msd_com);
		//msd[dt][iatom][0, x1,y1,z1,r1, x2,y2,z2,r2...]: 0 for average of Num files, (dimension+1)for sum
		msd_point(ifile, rAtom, msd);
		for (int dt = 1; dt <= m_frames[0][1]; dt++)
		{
			for (int i = 0; i < NumChains; i++)
			{
				msd_ave[dt-1][i][0] = 0; 
				for (int j = 0; j < nChain; j++)
				{
					int k = i * nChain + j;
					//msd_ave[dt][ichain][0, r1,r2,r3 ...]: 0 for average of N beeds
					msd_ave[dt-1][i][j+1] = msd[dt-1][k][0];
					msd_ave[dt-1][i][0] += msd_ave[dt-1][i][j+1];
				}
				msd_ave[dt-1][i][0] /= nChain;
			}
		}
	}
	else 
	{
		cout << "Wrong MSD Label!" << endl;
		exit(1);
	}
	return msd_ave;
}

//output
void LmpFile::out_msd(const string foutname, const vec_doub3 vec_com, const vec_doub3 vec_ave, const vector<string> &label)
{
	double time;
	//num_chains
	int NumChains = vec_com[0].size();
	int nChain = vec_ave[0][0].size() - 1;
	ofstream fout(foutname);
	fout << "time ";
	cout << "time "; 
	//output << "time ";
	for (int ichain = 0; ichain < NumChains; ichain++)
	{
		if (label[0] == "com" || label[0] == "COM")
		{
			if (label[1] != "cut")
			{
				for (int ifile = 0; ifile < m_files[0]; ifile++)
				{
					cout << "msd[" << ichain + 1 << "][" << m_fnamebel[ifile][1] << "] ";
					//output << "msd[" << ichain + 1 << "][" << m_fnamebel[ifile][1] << "] ";
					fout << "msd[" << ichain + 1 << "][" << m_fnamebel[ifile][1] << "] ";
				}
			}
			if (m_files[0] > 1 || label[1] == "cut")
			{
				cout << "ave[" << ichain + 1 << "][files] ";
				//output << "ave[" << ichain + 1 << "] ";
				fout << "ave[" << ichain + 1 << "][files] ";				
			}

		}
		else if (label[0] == "ave" || label[0] == "AVE")
		{
			if (label[1] != "cut")
			{
				for (int j = 0; j < nChain; j++)
				{
					cout << "msd[" << ichain + 1 << "][" << j + 1 << "] ";
					//output << "msd[" << ichain + 1 << "][" << j + 1 << "] ";
					fout << "msd[" << ichain + 1 << "][" << j + 1 << "] ";
				}
			}
			cout << "ave[" << ichain + 1 << "][atoms] ";
			//output << "ave[" << ichain + 1 << "] ";
			fout << "ave[" << ichain + 1 << "][atoms] ";				
		}
		else if (label[0] == "all" || label[0] == "\0")
		{
			cout << "msd[" << ichain + 1 << "][com] "
			<< "msd[" << ichain + 1 << "][ave] ";
			//output << "msd[" << ichain + 1 << "][" << j + 1 << "] ";
			fout << "msd[" << ichain + 1 << "][com] "
			<< "msd[" << ichain + 1 << "][ave] ";
		}
		else
		{
			cout << "Wrong MSD Label!" << endl;
			exit(1);
		}
	}
	cout << endl;
	fout << endl;

	//extern int Max_frame = 20000;int framestep = 5000; double md_dt = 0.001;
	for (int dt = 1; dt <= m_frames[0][1]; dt++)
	{
		time = dt * framestep * md_dt;
		cout << time << " ";
		//output << time << " ";
		fout << time << " ";
		for(int i = 0; i < NumChains; i++)
		{
			if (label[0] == "com" || label[0] == "COM")
			{	
				if (label[1] != "cut")
				{
					for (int ifile = 0; ifile < m_files[0]; ifile++)
					{	
						if (m_fnamebel[ifile][1] == "  ")
							continue;
						int j = (dimension + 1) * ifile + 1; 
						if (m_fnamebel[ifile][1] == "000" && dt > m_frames[ifile + 1][1])	//suplement
						{
							cout << "nan ";
							//output << "nan ";
							fout << "nan ";
						}
						else
						{
							cout << vec_com[dt-1][i][j+dimension] << " ";
							//output << vec_com[dt-1][i][j+dimension] << " ";
							fout << vec_com[dt-1][i][j+dimension] << " ";
						}	
					}
				}
				if (m_files[0] > 1 || label[1] == "cut")
				{
					cout << vec_com[dt-1][i][0] << " ";		// << " " << vec_com[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
					//output << vec_com[dt-1][i][0];		// << " " << vec_com[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
					if (m_files[1] != 1)
						fout << vec_com[dt-1][i][0] << " "; 	// << " " << vec_com[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
				}
			}	
			else if (label[0] == "ave" || label[0] == "AVE")
			{
				if (label[1] != "cut")
				{
					for (int j = 0; j < nChain; j++)
					{
						cout << vec_ave[dt-1][i][j+1] << " ";
						fout << vec_ave[dt-1][i][j+1] << " ";
					}
				}
				cout << vec_ave[dt-1][i][0] << " ";
				fout << vec_ave[dt-1][i][0] << " ";
				
			}
			else if (label[0] == "all" || label[0] == "ALL" || label[0] == "\0")
			{
				cout << vec_com[dt-1][i][0] << " " << vec_ave[dt-1][i][0] << " ";
				fout << vec_com[dt-1][i][0] << " " << vec_ave[dt-1][i][0] << " ";
			}
			else
			{
				cout << "Wrong MSD Label!" << endl;
				exit(1);
			}
		}
		
		cout << endl;
		//output << endl;
		fout << endl;
	}
	fout.close();
	
}

//read atom information -> atom[iframe][jatom][kinfo]
string read_atoms(ifstream &fin, int iframe, int nAtoms, vec_doub3 &vec)
{
	string temp;
	stringstream ss;
	string head;
	int Num_atoms = 1;
	int Num_obs = 1;
	
	for (int clear = 0; clear < 2; clear++)		//clear
		getline(fin, temp);
	ss << temp;
	ss >> Num_atoms;							//Number of atoms
	ss.clear();
	Num_obs = Num_atoms - nAtoms;				//for clearing obstacles
	if (Num_obs < 0)
	{
		cout << "Wrong atoms!" << endl;
		exit(1);
	}
	
	for (int clear = 0; clear < 5; clear++)			
		getline(fin, temp);
	head = temp;								//the head
	
	for (int j = 0; j < nAtoms; j++)
	{
		getline(fin, temp);
		ss << temp;
		//extern const int Num_info = 15
		for (int k = 0; k < Num_info; k++)
			ss >> vec[iframe][j][k];
		ss.clear();
	}

	for (int clear = 0; clear < Num_obs; clear ++)	//clear information of obstacles
		getline(fin, temp);
	return head;
}
