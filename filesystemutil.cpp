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
    } else if (fd >= 12 && fd < 16) {
        return 4;
    } else if (fd >= 16 && fd < 20) {
        return 5;
    } else if (fd >= 20 && fd < 24) {
        return 6;
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

int FileSystemSim::setFileDes(int fd, int32_t len, int32_t blk1,
                               int32_t blk2, int32_t blk3)
{
    int32_t fdBuffer[16];
    int fdLoc = (fd % 4) * 4; /* Gets relative position to block */
    int fdBlock = getBlockNum(fd); /* Returns block number */
    if (fdBlock == -1) {
        DEBUG("Invalid fd: " << fd << endl);
        return -1;
    }
    
    /* read then write */
    disk.read_block(fdBlock, fdBuffer);
    fdBuffer[fdLoc] = len;
    fdBuffer[fdLoc + 1] = blk1;
    fdBuffer[fdLoc + 2] = blk2;
    fdBuffer[fdLoc + 3] = blk3;
    disk.write_block(fdBlock, fdBuffer);
    
    return 0;
}

int FileSystemSim::getFileDes(int fd, int32_t *len, int32_t *blk1,
                               int32_t *blk2, int32_t *blk3)
{
    int32_t fdBuffer[16];
    int fdLoc = (fd % 4) * 4;
    int fdBlock = getBlockNum(fd);
    if (fdBlock == -1) {
        DEBUG("Invalid fd: " << fd << endl);
        return -1;
    }
    
    /* read disk */
    disk.read_block(fdBlock, fdBuffer);
    *len = fdBuffer[fdLoc];
    *blk1 = fdBuffer[fdLoc + 1];
    *blk2 = fdBuffer[fdLoc + 2];
    *blk3 = fdBuffer[fdLoc + 3];
    
    return 0;
}

int FileSystemSim::getOpenFD()
{
    int32_t fdBuffer[16];
    
    /* read each block */
    for (int i = 1; i < 7; i++) {
        disk.read_block(i, fdBuffer);
        /* read each file descriptor in each block */
        for (int j = 0; j < 4; j++) {
            if (fdBuffer[j * 4] == -1)
                return ((i - 1) * 4) + j;
        }
    }
    
    /* All FD taken */
    return -1;
}

int FileSystemSim::changeBuffer(int index, int disk)
{
    int32_t len, blk1, blk2, blk3;
    
    if (getFileDes(OFT[index].fdIndex, len, blk1, blk2, blk3))
        return -1;
    
    /* Write current buffer back to disk block */
    disk.write_block(OFT[index].currBlock, OFT[index].buffer);
    /* Read new block to buffer */
    switch(disk){
        case 0:
            if (blk1 == -1) {
                DEBUG("Block not inited\n");
            }
            disk.read_block(blk1, OFT[index].buffer);
            OFT[index].currBlock = blk1;
            break;
        case 1:
            if (blk2 == -1) {
                DEBUG("Block not inited\n");
            }
            disk.read_block(blk2, OFT[index].buffer);
            OFT[index].currBlock = blk2;
            break;
        case 2:
            if (blk3 == -1) {
                DEBUG("Block not inited\n");
            }
            disk.read_block(blk3, OFT[index].buffer);
            OFT[index].currBlock = blk3;
            break;
        default:
            DEBUG("WHYY(Unable to change buffers\n");
            return -1;
    }
    return 0;
}

char FileSystemSim::_readByte(int index)
{
    if (OFT[index].fileLen < OFT[index].currPos) {
        DEBUG("PANIC!!\n");
        return -1;
    }
    /* if we are at the beginning of each disk */
    if ((OFT[index].currPos % 64) == 0)
        changeBuffer(index, (OFT[index].currPos / 64)
    
}

int FileSystemSim::_readFile(int index, int len, char *printBuffer)
{
    if (printBuffer == NULL) {
        DEBUG("Read buffer not initialized\n");
        return -1;
    }
    
    if (OFT[index].fdIndex == -1) {
        cout << "error ";
        DEBUG("File descriptor not open\n");
        return -1;
    }
    
    int readCount = 0;
    while(readCount != len) {
        /* End of file */
        if (OFT[index].fileLen == OFT[index].currPos)
            break;
        printBuffer[readCount] = OFT[index].buffer[currPos];
        readCount++;
        OFT[index].currPos++;
    }
    
    return readCount;
}

void FileSystemSim::dumpOFT()
{
    cout << "-------OFT----------" << endl;
    cout << "fd Index Len CurrPos currBlock Buffer" << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << i << "    "
        << OFT[i].fdIndex << "   "
        << OFT[i].fileLen << "     "
        << OFT[i].currPos << "    "
        << OFT[i].currBlock << "    ";
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
        OFT[i].currBlock = 0;
        OFT[i].currPos = 0;
        OFT[i].fdIndex = -1;
        OFT[i].fileLen = 0;
    }
}

