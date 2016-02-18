#include "diskSim.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
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
    
    /* Initialize OFT */
    initOFT();
    
    /* Initialize Directory Descriptor (0)*/
    setFileDes(0, 0, -1, -1, -1);
    
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
    
    if (getFDDir(name, 0) != -1) {
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
    setFileDes(fd, 0, -1, -1, -1);
    setFDDir(name, fd);
    
    return 0;
    
}

int FileSystemSim::open(char *name)
{
    int diskFD;
    int openFD = -1;
    
    for (int i = 1; i < 4; i++) {
        if (OFT[i].fdIndex == -1) {
            openFD = i;
            break;
        }
    }
    
    if (openFD == -1) {
        cout << "error ";
        DEBUG("Too many files open!" << endl);
        return -1;
    }
    
    diskFD = getFDDir(name, 0);
    if (diskFD == -1) {
        cout << "error ";
        DEBUG("File not found! " << name << endl);
        return -1;
    }
    
    /* check if file already open */
    for (int i = 1; i < 4; i++) {
        if(OFT[openFD].fdIndex == diskFD) {
            cout << "error ";
            DEBUG("File already open\n");
            return -1;
        }
    }
    int len, blk1, blk2, blk3;
    
    getFileDes(diskFD, &len, &blk1, &blk2, &blk3);
    
    OFT[openFD].currPos = 0;
    OFT[openFD].fdIndex = diskFD;
    OFT[openFD].fileLen = len;
    OFT[openFD].currBlock = blk1;
    
    cout << name << " opened " << openFD << " ";
    
    return  0;
    
}

int FileSystemSim::del(char *name)
{
    int diskFD;
    
    diskFD = getFDDir(name, 1);
    if (diskFD == -1) {
        cout << "error ";
        DEBUG("file does not exist\n");
        return -1;
    }
    int len, blk1, blk2, blk3;
    
    /* Remove from disk */
    getFileDes(diskFD, &len, &blk1, &blk2, &blk3);
    
    removeBlock(blk1);
    removeBlock(blk2);
    removeBlock(blk3);
    
    setFileDes(diskFD, -1, -1, -1, -1);
    
    /* Remove from OFT if exist */
    for (int i = 1; i < 4; i++) {
        if (OFT[i].fdIndex == diskFD) {
            OFT[i].fdIndex = -1;
            OFT[i].fileLen = -1;
            OFT[i].currPos = 0;
            memset(OFT[i].buffer, '\0', 64);
        }
    }
    
    return 0;
}

int FileSystemSim::read(int index, int len)
{
    int readCount;
    char buffer[len + 1];
    
    readCount = _readFile(index, len, buffer);
    
    buffer[readCount] = '\0';
    if (readCount > 0)
        cout << buffer << " ";
    
    return readCount;
    
}

int FileSystemSim::write(int index, char byte, int len)
{
    int writeCount;
    char buffer[len];
    
    for (int i = 0; i < len; i++)
        buffer[i] = byte;
    
    writeCount = _writeFile(index, len, buffer);
    
    
    if (writeCount != -1)
        cout << writeCount << " bytes written ";
    
    return writeCount;
}

int FileSystemSim::close(int index)
{
    if (OFT[index].fdIndex == -1) {
        cout << "error ";
        DEBUG("FD already closed\n");
        return -1;
    }
    
    int len, blk1, blk2, blk3;
    getFileDes(OFT[index].fdIndex, &len, &blk1, &blk2, &blk3);
    
    setFileDes(OFT[index].fdIndex, OFT[index].fileLen, blk1, blk2, blk3);
    disk.write_block(OFT[index].currBlock, OFT[index].buffer);
    
    OFT[index].currPos = 0;
    OFT[index].fdIndex = -1;
    OFT[index].fileLen = 0;
    OFT[index].currBlock = 0;
    memset(OFT[index].buffer, 0, 64);
    
    cout << index << " closed ";
    
    return 0;
}

int FileSystemSim::seek(int index, int pos)
{
    if (pos < 0) {
        cout << "error ";
        DEBUG("Seek position out of bounds\n");
        return -1;
    }
    
    /* If position is past filelen, put it to end of file */
    if (pos > OFT[index].fileLen) {
        pos = OFT[index].fileLen;
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
    
    cout << "position is " << OFT[index].currPos << " ";
    return 0;
}

int FileSystemSim::printFiles()
{
    char rbuf[4];
    
    OFT[0].currPos = 0;
    
    while (OFT[0].currPos < OFT[0].fileLen) {
        _readFile(0, 4, rbuf);
        if (!(rbuf[0] == 0)) {
            cout << rbuf << " ";
        }
        seek(0, OFT[0].currPos + 4);
    }
    
    return 0;
}

int FileSystemSim::save(char *name)
{
    FILE *saveFile;
    int32_t buffer[BLOCK_SIZE];
    
    if (name == NULL) {
        DEBUG("No file specified");
        return -1;
    }
    
    for(int i = 0; i < 4; i++) {
        if (OFT[i].fdIndex != -1)
            close(i);
    }
    
    saveFile = fopen(name, "w");
    
    for (int i = 0; i < LDISK_SIZE; i++) {
        disk.read_block(i, buffer);
        fwrite(buffer, 4, BLOCK_SIZE, saveFile);
    }
    
    fclose(saveFile);
    
    return 0;
}

int FileSystemSim::load(char *name)
{
    FILE *openFile = NULL;
    int32_t buffer[BLOCK_SIZE];
    
    openFile = fopen(name,"r");
    if (openFile == NULL){
        FileSystemSim();
        return 0;
    }
    for (int i = 0; i < LDISK_SIZE; i++) {
        fread(buffer, 4, BLOCK_SIZE, openFile);
        disk.write_block(i, buffer);
    }
    fclose(openFile);
    
    int len, blk1, blk2, blk3;
    
    getFileDes(0, &len, &blk1, &blk2, &blk3);
    OFT[0].fileLen = len;
    
    return 0;
}
