//Ho Choi 53734155
#ifndef TLB_H
#define TLB_H

class TLB
{
public:
	TLB();
	~TLB();
	int find(int s, int p);
	int update(int index, int s, int p, int f);
	int get_f(int index);
	void init();
private:
	int** table;
};



#endif