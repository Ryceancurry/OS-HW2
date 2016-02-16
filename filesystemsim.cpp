#include "diskSim.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

void FileSystemSim::dump()
{
    disk.dump_disk();
}

FileSystemSim::FileSystemSim()
{
    /* Set the bitmask - Bit 0-6 are taken by default */
    uint64_t bitmap = 0x7f;
    setBitMap(bitmap);x

    /* Initalize file descriptors - (1-23) */
    for (int i = 1; i < 24; i++) {
        setFileDes(i, -1, -1, -1, -1);
    }
    
    /* Initialize Directory Descriptor (0)*/
    setFileDes(0, 0, 0, 0, 0);
    
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
    
    int fd;
    
    fd = getOpenFD();
    if (fd == -1) {
        cout << "error ";
        DEBUG("No more file descriptors");
        return -1;
    }
    
    setFileDes(fd, 0, 0, 0, 0);
    
    return 0;
    
}
