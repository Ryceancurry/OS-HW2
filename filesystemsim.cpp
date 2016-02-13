#include "diskSim.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

static int getBlockNum(int fd)
{
    /* Find Block */
    if ( fd >= 0 && fd < 4) {
        return 1;
    } else if (fd >= 4 && fd < 8) {
        return 2;
    } else if (fd >= 8 && fd < 12) {
        return 3;
    } else if (fd >= 16 && fd < 20) {
        return 4;
    } else if (fd >= 20 && fd < 24) {
        return 5;
    } else {
        return -1;
    }
}

uint64_t FileSystemSim::getBitMap()
{
    uint64_t bitmap[8];

    disk.read_block(0, bitmap);

    return bitmap[0];

}

void FileSystemSim::setBitMap(uint64_t bm)
{
    uint64_t bitmap[8];

    bitmap[0] = bm;
    disk.write_block(0, bitmap);
}

void FileSystemSim::setFileDes(int fd, uint32_t len, uint32_t blk1,
                                uint32_t blk2, uint32_t blk3)
{
    uint32_t fdBuffer[16];
    int fdLoc = (fd % 4) * 4; /* Gets relative position to block */
    int fdBlock = getBlockNum(fd); /* Returns block number */
    if (fdBlock == -1) {
        DEBUG("Invalid fd: " << fd);
        return;
    }
    
    /* read then write */
    disk.read_block(fdBlock, fdBuffer);
    fdBuffer[fdLoc] = len;
    fdBuffer[fdLoc + 1] = blk1;
    fdBuffer[fdLoc + 2] = blk2;
    fdBuffer[fdLoc + 3] = blk3;
    disk.write_block(fdBlock, fdBuffer);
}

FileSystemSim::FileSystemSim()
{
    /* Set the bitmask - Bit 0-6 are taken by default */
    uint64_t bitmap = 0x7f;
    setBitMap(bitmap);

    /* Set file descriptors - (1-24) */
    
}
