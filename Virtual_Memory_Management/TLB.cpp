#include "TLB.h"
TLB::TLB()
{
	table = new int*[4];
	init();
}
void TLB::init()
{
	for (int i = 0; i < 4; i++)
	{
		table[i] = new int[4];
		table[i][0] = -1;
		table[i][1] = -1;
		table[i][2] = -1;
		table[i][3] = -1;
	}
}
TLB::~TLB()
{
	delete[] table;
}
int TLB::find(int s, int p) // works as pop?
{
	for (int i = 0; i < 4; i++)
	{
		if ((table[i][1] == s) && (table[i][2] == p))
		{
			return i;
		}
	}
	return -1;
}
int TLB::update(int index, int s, int p, int f)
{
	if (index == -1)//miss case
	{
		for (int i = 0; i < 4; i++)
		{
			if (table[i][0] == -1)//find empty spot (least latest acessed.)
			{
				index = i;
				break;
			}
		}
		if (index == -1)//if all occupied
		{
			for (int i = 0; i < 4; i++)
			{
				if (table[i][0] == 0)//find lowest priority (least latest acessed.)
				{
					index = i;
					break;
				}
			}
		}
		table[index][0] = 0;//all shall fall by this index
		table[index][1] = s;
		table[index][2] = p;
		table[index][3] = f;
	}
	for (int i = 0; i < 4; i++)
	{
		if (table[i][0] > table[index][0])
		{
			table[i][0]--;
		}
	}
	table[index][0] = 3;
	

	return index;
}

int TLB::get_f(int index)
{
	return table[index][3];
}