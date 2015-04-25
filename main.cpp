#include "pru.h"
int main()
{
	Pru *pru1,*pru2;
	pru1 = Pru::getInstance("example.bin");
	pru2 = Pru::getInstance();

	if(pru1->pruStart())
		cout << "Pru Started" << endl;
	int i;
	for(i = 0; i < 5000; i++)
	{
		unsigned int v;
		pru1->writeRAM(34,i,0);
		pru1->readRAM(&v,i,0);
		if(v != 34) cout << "oops" << endl;
	}

	if(pru1->pruWaitHalt())
		cout << "Pru Halted" << endl;

	pru1->~Pru();
	pru2->~Pru();
	return 0;
}
