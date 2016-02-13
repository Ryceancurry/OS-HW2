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

int64_t FileSystemSim::getBitMap()
{
    int64_t bitmap[8];

    disk.read_block(0, bitmap);

    return bitmap[0];

}

void FileSystemSim::setBitMap(int64_t bm)
{
    int64_t bitmap[8];
    
    for(int i = 1; i < 8; i++)
        bitmap[i] = 0;
    
    bitmap[0] = bm;
    disk.write_block(0, bitmap);
}

void FileSystemSim::setFileDes(int fd, int32_t len, int32_t blk1,
                                int32_t blk2, int32_t blk3)
{
    int32_t fdBuffer[16];
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

void FileSystemSim::getFileDes(int fd, int32_t *len, int32_t *blk1,
                                int32_t *blk2, int32_t *blk3)
{
    int32_t fdBuffer[16];
    int fdLoc = (fd % 4) * 4;
    int fdBlock = getBlockNum(fd);
    if (fdBlock == -1) {
        DEBUG("Invalid fd: " << fd);
        return;
    }
    
    /* read disk */
    disk.read_block(fdBlock, fdBuffer);
    *len = fdBuffer[fdLoc];
    *blk1 = fdBuffer[fdLoc + 1];
    *blk2 = fdBuffer[fdLoc + 2];
    *blk3 = fdBuffer[fdLoc + 3];
}

void FileSystemSim::dumpOFT()
{
    cout << "-------OFT----------" << endl;
    cout << "fd Index Len CurrPos Buffer" << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << i << "    "
        << OFT[i].fdIndex << "   "
        << OFT[i].fileLen << "     "
        << OFT[i].currPos << "     ";
        for (int j = 0; j < 64; j++) {
            cout << static_cast<int16_t>(OFT[0].buffer[0]);
        }
        cout << endl;
    }
}

void FileSystemSim::initOFT()
{
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0;j < 64; j++)
            OFT[i].buffer[j] = 0;
        OFT[i].currPos = 0;
        OFT[i].fdIndex = -1;
        OFT[i].fileLen = 0;
    }
}

FileSystemSim::FileSystemSim()
{
    /* Set the bitmask - Bit 0-6 are taken by default */
    uint64_t bitmap = 0x7f;
    setBitMap(bitmap);

    /* Initalize file descriptors - (1-23) */
    for (int i = 1; i < 24; i++) {
        setFileDes(i, -1, -1, -1, -1);
    }
    
    /* Initialize Directory Descriptor (0)*/
    setFileDes(0, 0, -1, -1, -1);
    
    /* Initialize OFT */
    initOFT();
    /* Setting Directory to Descriptor 0 */
    OFT[0].currPos = 0;
    OFT[0].fdIndex = 0;
    OFT[0].fileLen = 0;
    
    /* Debugging */
    dumpOFT();
    disk.dump_disk();
    
}
