#include "diskSim.h"
#include <string.h>
#include <iostream>
using namespace std;

LDisk::LDisk()
{
    /* init disk to 0 */
    for (int i = 0; i < LDISK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            ldisk[i][j] = 0;
}

void LDisk::read_block(int i, void *p)
{
	memcpy(p, ldisk[i], 64);
}

void LDisk::write_block(int i, void *p)
{
	memcpy(ldisk[i], p, 64);
}

void LDisk::dump_disk()
{
    printf("bitmap:%x%x\n", ldisk[0][0], ldisk[0][1]);
    for (int i = 0; i < LDISK_SIZE; i++) {
        cout << "Block " << i << "     ";
        for (int j = 0; j < BLOCK_SIZE; j++) {
            cout << ldisk[i][j] << " ";
        }
        cout << endl;
    }
}


