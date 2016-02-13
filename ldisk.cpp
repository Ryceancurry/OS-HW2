#include "diskSim.h"
#include <string.h>

void LDisk::read_block(int i, void *p)
{
	memcpy(p, ldisk[i], 64);
}

void LDisk::write_block(int i, void *p)
{
	memcpy(ldisk[i], p, 64);
}


