#include "diskSim.h"
#include <iostream>
#include <string.h>

using namespace std;

void printBlock(char *block)
{
	for(int i = 0; i < 64; i++)
	{
		cout << block[i] << " ";
	}
	cout << endl;
}

int main()
{
	LDisk testDisk;
	char block1[64], block2[64];

	cout << "Setting block1 to 0" << endl;
	memset(block1, '0', 64);
	block1[5] = '6';
	block1[63] = '9';
	printBlock(block1);
	cout << "Setting block2 to 1" << endl;
	memset(block2, '1', 64);
	printBlock(block2);
	
	cout << "Writing to LDisk block1" << endl;
	testDisk.write_block(0, block1);

	cout << "Read Ldisk into block2" << endl;
	testDisk.read_block(0, block2);

	cout << "Reading Block2" << endl;
	printBlock(block2);
	
	return 0;
}
