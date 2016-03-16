#ifndef MANAGER_H
#define MANAGER_H
// Ho Choi 53734155
/*
There is only one process hence only one ST(Segment Table). Each Entry of ST points to PT (Page Table) which points to program/data pages.
PM[s] = p, PM[p] = datapage. PM[p]+w = data.

Virtual Address: VA = 32bits , 9s, 10p, 9w. ergo size = 2^9, 2^10, 2^9.
Physical Memory: PM = Array of Integers. frames of size 512 integers. 1024 frames. 
Physical Address: address of physical memory, size of PA is 19 bit. 2^9 * 2^10. (frame size * number of frames)
Segment Table: ST = 512 integers (1 frame). Always resides in frame 0, never be paged out.
Page Tables: PT = 1024 integers (2 frames, consecutive).
program/data: w = 512 integers (1 frame).
bitmap = 32 bit

*/

#include "TLB.h"
#include <iostream>
#include <bitset>
#include <algorithm>
#include <string>
#include <exception>

using namespace std;
const int s_size = 512;
const int p_size = 1024;
const int w_size = 512;
const int frame_size = 512;
const int frame_n = 1024;
const int pm_size =  frame_n * frame_size;
const int pa_size = 19;
const int bitmap_n = frame_n / 32;
const int page_fault = -1;
const int empty = 0;

class Manager
{
public:
	Manager();
	~Manager();

	int init();
	int init(int s, int f);
	int init(int p, int s, int f);

	int encode(int s, int p, int w);
	int* translate(int VA);
	int read(int VA);
	int read(int s, int p, int w);
	int write(int VA);
	int write(int s, int p, int w);
	string read_TLB(int VA);
	string write_TLB(int VA);

	int alloc_PT();
	int alloc_data();
	int alloc_bitmap(int address);
	int free_bitmap(int address);

	void show_bitmap();
private:
	//VA =[0	][512	][1024	][1536	][2048	]...
	//bm =[0	][1		][2		][3		][4		]...
	int* spw;
	int* PM;		//physical memory.
	int* Bitmap;	//
	int* mask;		//
	TLB* tlb;
};

#endif // !MANAGER_H
