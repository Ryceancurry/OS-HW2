#ifndef LDISK_H
#define LDISK_H

#include <stdint.h>

#define DEBUG_BUILD

#ifdef DEBUG_BUILD
# define DEBUG(x) do { std::cerr << x; } while (0)
#else
# define DEBUG(x) do {} while (0)
#endif

/* Constants */
#define BLOCK_SIZE	16
#define LDISK_SIZE	64

struct OpenFileTable {
    int8_t buffer[64];
    int currBlock; /* current block held */
    int currPos;
    int fdIndex;
    int fileLen;
};

class LDisk
{
	int32_t ldisk[LDISK_SIZE][BLOCK_SIZE];

public:
    LDisk();
    /*
     * @param i Block location
     * @param *p Pointer to array to be filled. Must be 64 bytes.
     * Reads the block at the given 'i' and populates 64 byte array with
     * data.
     */
    void read_block(int i, void *p);

    /*
     * @param i Block location
     * @param *p Pointer to array to be written to disk. Must be 64 bytes.
     * Writes the block at the given 'i' with the data in the 64 byte array
     */
    void write_block(int i, void *p);

    /* debugging */
    void dump_disk();
};

class FileSystemSim
{
    /* Core */
	LDisk disk;
	struct OpenFileTable OFT[4];

public:
    FileSystemSim();
    
    void dump();
    int createFile(char *name);
    
private:
    /* Read */
    int _readFile(int index, int len, char *printBuffer);
    /* Read next byte and increments currPos, changes buffer if necessary */
    char _readByte(int index);
    /* Change OFT buffer */
    int changeBuffer(int index, int disk);
    
    /* BitMap */
    int64_t getBitMap();
    void setBitMap(int64_t bm);
    
    /* File Descriptors */
    int setFileDes(int num, int32_t len, int32_t blk1, int32_t blk2, int32_t blk3);
    int getFileDes(int num, int32_t *len, int32_t *blk1, int32_t *blk2, int32_t *blk3);
    int getOpenFD();
    
    /* File Name */
    /* get FD number with file name, return -1 if file does not exist */
    int getFDDir(char *name);
    /* set filename in directory */
    int setFDDir(char *name);
    
    /* OFT */
    void initOFT();
    
    /* Debuggin */
    void dumpOFT();
};
#endif
