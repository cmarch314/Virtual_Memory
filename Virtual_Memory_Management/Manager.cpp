//Ho Choi 53734155
#include "Manager.h"

//constructor.
Manager::Manager()
{
	tlb = new TLB();
	spw = new int[3];	//stores s,p,w
	PM = new int[pm_size]; // PM[2^19]
	Bitmap = new int[32]; //bitmap = 32*32 bit = 1024 bit, : 1024 frames
	mask = new int[32];//32 bit mask
	////32 bit map mask generating//// mask[0]  = 1000 0000 ... 0000 0000
	// mask[1]  = 0100 0000 ... 0000 0000
	for (int i = 31; i >= 0; i--)	// mask[2]  = 0010 0000 ... 0000 0000
	{								// mask[30] = 0000 0000 ... 0000 0010
		mask[i] = 1 << (31 - i);	// mask[31] = 0000 0000 ... 0000 0001
	}								// 
	////////////////////////////////// * mask is an array of unsigned integer that holds a bit of corresponding position.
	init();
}

int Manager::init()
{
	//initialize bitmap
	Bitmap[0] = mask[0]; // bitmap = 1000 0000 0000 ... 0000 , first bit(frame) is allocated (reserved) to s.

	for (int i = 1; i < 32; i++)
	{
		Bitmap[i] = 0; // bitmap[1-31] = 0000 0000 0000 ... 0000, all free
	}
	//initialize PM
	for (int i = 0; i < 1024; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			PM[i*512 + j] = 0; //all free memory.
		}
	}
	return 0;
}

int Manager::init(int s, int f)
{

	PM[s] = f;
	//PT takes two consecutive frame.
	if (f != -1)
	{
		alloc_bitmap(f);
		alloc_bitmap(f + 512);
	}
	return 0;
}

int Manager::init(int p, int s, int f)
{
	PM[PM[s] + p] = f;
	if (f != -1)
	{
		alloc_bitmap(f);
	}
	return 0;
}

//Demolitionar
Manager::~Manager()
{
	delete[] mask;
	delete[] PM;
	delete[] spw;
	delete[] Bitmap;
	delete this;
}

//translate VA to s, p, w.
int* Manager::translate(int VA)
{
	int s = 0, p = 0, w = 0;
	//translate bits: VA to s,p,w using mask.
	// VA [0 1 2 3  4  5  6  7  8  9  10 11 12   13 14 15 16 17 18 19 20 21 22  23 24 25 26 27 28 29 30 31 ]
	// PA [	 N/A  ][0  1  2  3  4  5  6  7  8  ][0  1  2  3  4  5  6  7  8  9 ][0  1  2  3  4  5  6  7  8  ]  
	//spw		   [23 24 25 26 27 28 29 30 31 ][22 23 24 25 26 27 28 29 30 31][23 24 25 26 27 28 29 30 31 ]
	//						s							p								w
	//					1frame,512,9bit			2frame,1024,10bit				1frame,512,9bit
	for (int i = 0; i < 9; i++)
	{
		// mask[0-3] not used on VA, 
		//for s mask[4 - 12]

		if ((VA & mask[i + 4]) != 0)
		{
			s = s | mask[i+23];
		}
		//for p
		if ((VA & mask[i + 13]) != 0)
		{
			p = p | mask[i+22];
		}
		//for w
		if ((VA & mask[i + 23]) != 0)
		{
			w = w | mask[i+23];
		}
	}
	if ((VA & mask[22]) != 0) // i == 9 , which is p9
	{
		p = p | mask[31];
	}
	
	spw[0] = s;	spw[1] = p;	spw[2] = w;
	return spw;
}

int Manager::encode(int s, int p, int w)
{
	int VA=0;
	//translate bits: VA to s,p,w using mask.
	// VA [0 1 2 3  4  5  6  7  8  9  10 11 12   13 14 15 16 17 18 19 20 21 22  23 24 25 26 27 28 29 30 31 ]
	// PA [	 N/A  ][0  1  2  3  4  5  6  7  8  ][0  1  2  3  4  5  6  7  8  9 ][0  1  2  3  4  5  6  7  8  ]  
	//spw		   [23 24 25 26 27 28 29 30 31 ][22 23 24 25 26 27 28 29 30 31][23 24 25 26 27 28 29 30 31 ]
	//						s							p								w
	//					1frame,512,9bit			2frame,1024,10bit				1frame,512,9bit
	for (int i = 0; i < 9; i++)//loop through 0 - 8
	{
		// mask[0-3] not used on VA, 
		//for s mask[4 - 12]

		if ((s & mask[i + 23]) != 0)
		{
			VA = VA | mask[i + 4];
		}
		//for p
		if ((p & mask[i + 22]) != 0)
		{
			VA = VA | mask[i + 13];
		}
		//for w
		if ((w & mask[i + 23]) != 0)
		{
			VA = VA | mask[i + 23];
		}
	}
	if ((p & mask[31]) != 0) // adding case i = 9 for p, position of VA[22]
	{
		VA = VA | mask[22];
	}
	return VA;
}

int Manager::read(int VA)
{
	spw = translate(VA);
	int s = spw[0], p = spw[1],  w = spw[2];
	return read(s, p, w);
}
int Manager::read(int s, int p, int w)
{
	//check s
	if (PM[s] == page_fault)
	{
		throw exception("pf");
	}
	else if (PM[s] == empty)
	{
		throw exception("err");
	}
	//check p
	if (PM[PM[s] + p] == page_fault)
	{
		throw exception("pf");
	}
	else if (PM[PM[s] + p] == empty)
	{
		throw exception("err");
	}
	return PM[PM[s] + p] + w;
}
string Manager::read_TLB(int VA)
{
	spw = translate(VA);
	int s = spw[0], p = spw[1], w = spw[2];
	int index, f, PA; string out = "";
	index = tlb->find(s, p);
	if (index == -1)//if miss
	{
		out.append("m ");
		//check VA, if err or pf, no update TLB 
		//check s
		try
		{
			PA = read(s, p, w);
			//if valid PA, pop 0, push s,p,PA
			f = PM[PM[s] + p];
			tlb->update(index, s, p, f);
			out.append(to_string(PA));
		}
		catch (exception& e)
		{
			out.append(e.what());
		}
	}
	else// if hit
	{
		out.append("h ");
		//use f from TLB to form PA = f+w
		f = tlb->get_f(index);
		PA = f + w;
		//remove s,p then push s,p
		tlb->update(index, s, p, f);
		out.append(to_string(PA));
	}
	return out;

}

int Manager::write(int VA)
{
	spw = translate(VA);
	int s = spw[0], p = spw[1], w = spw[2];
	return write(s, p, w);
}
int Manager::write(int s, int p, int w)
{
	int pp = PM[s];
	int ww = PM[PM[s] + p];
	//check ST
	if (PM[s] == page_fault)
	{
		throw exception("pf");
	}
	else if (PM[s] == empty)
	{
		//allocate new empty 2 consecutive frame for PT fill with 0s, update the value. bitmap. and proceed.
		PM[s] = alloc_PT();
	}

	//check PT
	if (PM[PM[s] + p] == page_fault)
	{
		throw exception("pf");
	}
	else if (PM[PM[s] + p] == empty)
	{
		//allocate a new blank frame to PM[PM[s]+p] fill with 0s. update the value. 
		PM[PM[s] + p] = alloc_data();
	}
	return PM[PM[s] + p] + w;

}
string Manager::write_TLB(int VA)
{
	spw = translate(VA);
	int s = spw[0], p = spw[1], w = spw[2];
	int index, f, PA; string out = "";
	index = tlb->find(s, p);
	if (index == -1)//if miss
	{
		out.append("m ");
		//check VA, if err or pf, no update TLB 
		//check s
		try
		{
			PA = write(s, p, w);
			//if valid PA, pop 0, push s,p,PA
			f = PM[PM[s] + p];
			tlb->update(index, s, p, f); // if -1 it will find 0 and update it.
			out.append(to_string(PA));
		}
		catch (exception& e)
		{
			out.append(e.what());
		}
	}
	else//if hit
	{
		out.append("h ");
		//use f from TLB to form PA = f+w
		f = tlb->get_f(index);
		PA = f + w;
		//remove s,p then push s,p
		tlb->update(index, s, p, f);
		out.append(to_string(PA));
	}
	return out;
}
int Manager::alloc_bitmap(int address)
{
	if (address == -1)
	{
		return -1;
	}
	for (int i = 0; i < 512; i++)
	{
		PM[address + i] = 0;
	}
	address = address / 512;
	int x = address / 32;
	int y = address % 32;
	Bitmap[x] = (Bitmap[x] | mask[y]);

//	show_bitmap();
	return 0;
}

int Manager::free_bitmap(int address) //I assume this would not be used.
{
	address = address / 512;
	int x = address / 32;
	int y = address % 32;
	Bitmap[x] = Bitmap[x] & (~mask[y]);
	return 0;
}

int Manager::alloc_data()//bitmap is 32*(int) array, which contains 32*32 bit using 32-bit mask.
{
	for (int i = 0; i < 32*32; i++)						//for each 32-bitmap table
	{
		if ((Bitmap[i / 32] & mask[i % 32]) == 0)//if the bit is empty
		{
			alloc_bitmap(i * 512);
			return i * 512;
		}
	}
}

int Manager::alloc_PT()//bitmap is 32*(int) array, which contains 32*32 bit using 32-bit mask.
{
	for (int i = 0; i < 32*32 - 1; i++)						//for each 32-bitmap table
	{
		if ((Bitmap[i / 32] & mask[i % 32]) == 0)//if the bit is empty
		{
			if ((Bitmap[(i+1) / 32] & mask[(i+1) % 32]) == 0)//and if the next bit is also empty
			{
				alloc_bitmap(i * 512);
				alloc_bitmap((i+1) * 512);
				return i * 512;
			}
		}
	}
}

void Manager::show_bitmap()
{

	cout <<"	BM"<< bitset<32>(Bitmap[0]) << endl;
}