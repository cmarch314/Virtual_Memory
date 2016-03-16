//Ho Choi 53734155
#include "Manager.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{
	int Generate = 0; // Generating input2.txt type file.
	string PAtxt = "PA1.txt";
	string VAtxt = "VA1.txt";

	string location = "I:/";
	Manager* m = new Manager();
	Manager* m_tlb = new Manager();
	string ifname = "";
	string line, out;
	int r_w, s, w, p, VA, f;
	exception* e;
	cout << "Enter file name(.txt):";
	getline(cin, ifname);
	
	ifstream iFile_1(location + ifname + "1.txt", std::ifstream::in);
	ifstream iFile_2(location + ifname + "2.txt", std::ifstream::in);
	ifstream iFile_2_tlb(location + ifname + "2.txt", std::ifstream::in);
	ofstream oFile_1(location + "537341551.txt", std::ofstream::out);
	ofstream oFile_2(location + "537341552.txt", std::ofstream::out);
	/*
	ifstream iFile_1(location + "TroubleShoot.txt", std::ifstream::in);
	ifstream iFile_2(location + ifname + ".txt", std::ifstream::in);
	ifstream iFile_2_tlb(location + ifname + ".txt", std::ifstream::in);
	ofstream oFile_1(location + ifname + "_1.txt", std::ofstream::out);
	ofstream oFile_2(location + ifname + "_2.txt", std::ofstream::out);
	*/
	string::size_type sz;
	
	//read input1.txt//////////////////////////////////////////////////////////////////////
	try//read first line
	{
		getline(iFile_1, line);
		line = line.append(" ");
		while (true) //read pairs
		{
			s = stoi(line, &sz);
			line = line.substr(sz + 1);
			f = stoi(line, &sz);
			line = line.substr(sz + 1);
			m->init(s, f);
			m_tlb->init(s, f);
		}

	}
	catch (exception& e)
	{ //will be end of line
	}
	try
	{
		//accept three numbers per each time
		getline(iFile_1, line);
		line = line.append(" ");
		while (true)
		{
			p = stoi(line, &sz);
			line = line.substr(sz + 1);
			s = stoi(line, &sz);
			line = line.substr(sz + 1);
			f = stoi(line, &sz);
			line = line.substr(sz + 1);
			m->init(p, s, f);
			m_tlb->init(p, s, f);
		}
	}
	catch (exception& e)
	{ //will be end of line
	}

	//read input2.txt//////////////////////////////////////////////////////////////////////
	try
	{
		//accept a pair of numbers
		getline(iFile_2, line);
		line = line.append(" ");
		while (true)
		{
			r_w = stoi(line, &sz);
			line = line.substr(sz + 1);
			VA = stoi(line, &sz);
			line = line.substr(sz + 1);
			try
			{
				if (r_w == 0)//read
				{
					out = to_string(m->read(VA));
					oFile_1 << out << " ";
				}
				else if (r_w == 1)//write
				{
					out = to_string(m->write(VA));
					oFile_1 << out << " ";
				}
			}
			catch (exception& e)
			{
				oFile_1 << e.what() << " ";
			}
			
		}
	}
	catch (exception& e)
	{ //will be end of line
		//		cout <<"error:"<< e.what() << endl;
	}
	//read input2.txt//////////////////////////////////////////////////////////////////////
	try
	{
		//accept a pair of numbers
		getline(iFile_2_tlb, line);
		line = line.append(" ");
		while (true)
		{
			r_w = stoi(line, &sz);
			line = line.substr(sz + 1);
			VA = stoi(line, &sz);
			line = line.substr(sz + 1);
			try
			{
				if (r_w == 0)//read
				{
					out = m_tlb->read_TLB(VA);
					oFile_2 << out << " ";
				}
				else if (r_w == 1)//write
				{
					out = m_tlb->write_TLB(VA);
					oFile_2 << out << " ";
				}
			}
			catch (exception& e)
			{
				oFile_2 << e.what() << " ";
			}

		}
	}
	catch (exception& e)
	{ //will be end of line
		//		cout <<"error:"<< e.what() << endl;
	}



	if (Generate == 1)//if Generater is on.
	{
		int w_r, s, p, w; 
		ifstream fPA(location + PAtxt, std::ifstream::in);
		ofstream fVA(location + VAtxt, std::ofstream::out);
		try
		{
			// PA1.txt = "w_r s p w \n"
			// VA1.txt = "w_r VA"
			while (getline(fPA, line))
			{
				line = line.append(" ");
				w_r = stoi(line, &sz);
				line = line.substr(sz + 1);
				s = stoi(line, &sz);
				line = line.substr(sz + 1);
				p = stoi(line, &sz);
				line = line.substr(sz + 1);
				w = stoi(line, &sz);
				line = line.substr(sz + 1);
				cout << "Encode:" << s << "," << p << "," << w << "->";
				VA = m_tlb->encode(s,p,w);
				fVA << to_string(w_r) << " " << VA << " ";
			}
		}
		catch (exception& e)
		{
			cout << "	error:" << e.what() << endl;
			fVA << e.what() << " ";
		}
		fVA.close();
		fPA.close();
	}

	iFile_1.close();
	oFile_1.close();
	iFile_2.close();
	oFile_2.close();
	iFile_2_tlb.close();
	
	return -1;
}