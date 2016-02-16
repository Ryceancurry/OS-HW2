#include "diskSim.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

void FileSystemSim::dump()
{
    disk.dump_disk();
    dumpOFT();
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
    //dumpOFT();
    //disk.dump_disk();
    
}

int FileSystemSim::createFile(char *name)
{
    if (strlen(name) > 3) {
        cout << "error ";
        DEBUG("File Name too long: " << name);
        return -1;
    }
    
    if (getFDDir(name) != -1) {
        cout << "error ";
        DEBUG("File already exist: " << name);
        return -1;
    }
    
    int fd;
    fd = getOpenFD();
    if (fd == -1) {
        cout << "error ";
        DEBUG("No more file descriptors");
        return -1;
    }
    setFileDes(fd, 0, 0, 0, 0);
    setFDDir(name, fd);
    
    return 0;
    
}

int FileSystemSim::seek(int index, int pos)
{
    if (pos < 0 || pos > 192) {
        cout << "error ";
        DEBUG("Seek position out of bounds\n");
        return -1;
    }
    
    if (pos > OFT[index].fileLen) {
        cout << "error ";
        DEBUG("Seek position greater than file len\n");
        return -1;
    }
    
    int currBlock = OFT[index].currPos / 64;
    int nextBlock = (pos - 1) / 64;
    
    if (currBlock == nextBlock) {
        OFT[index].currPos = pos;
        return 0;
    }
    
    /* change buffer if not current disk */
    changeBuffer(index, nextBlock);
    OFT[index].currPos = pos;
    return 0;
}
