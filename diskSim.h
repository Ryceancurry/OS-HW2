#ifndef LDISK_H
#define LDISK_H

#include <stdint.h>

#ifdef DEBUG_BUILD
# define DEBUG(x) do { std:cerr << x; } while (0)
#else
# define DEBUG(x) do {} while (0)
#endif

/* Constants */
#define BLOCK_SIZE	16
#define LDISK_SIZE	64

struct OpenFileTable {
    char buffer[64];
    int currPos;
    int fdIndex;
    int fileLen;
};

class LDisk
{
	int ldisk[LDISK_SIZE][BLOCK_SIZE];

public:
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
};

class FileSystemSim
{
    /* Core */
	LDisk disk;
	struct OpenFileTable OFT[4];

public:
    FileSystemSim();
    
private:
    /* BitMap */
    uint64_t getBitMap();
    void setBitMap(uint64_t bm);
    
    /* File Descriptors */
    void setFileDes(int num, uint32_t len, uint32_t blk1, uint32_t blk2, uint32_t blk3);
    void getFileDes(int num, uint32_t *len, uint32_t *blk1, uint32_t *blk2, uint32_t *blk3);
    int getOpenFD();
    
};
#endif
