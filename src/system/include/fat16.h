#pragma once

#include <types.h>
#include <stdbool.h>

#define SECTOR_SIZE     512
#define ENTRY_AVAILABLE 0x00
#define ENTRY_ERASED    0xe5

typedef struct BiosParamBlock
{
    unsigned char   jump[3];
    unsigned char   oem[8];
    unsigned short  bytesPerSector;
    unsigned char   sectorsPerCluster;
    unsigned short  reservedSectorCount;
    unsigned char   fatCount;
    unsigned short  rootEntryCount;
    unsigned short  sectorCount;
    unsigned char   mediaType;
    unsigned short  sectorsPerFat;
    unsigned short  sectorsPerTrack;
    unsigned short  headCount;
    unsigned int    hiddenSectorCount;
    unsigned int    largeSectorCount;
    unsigned char   driveNumber;
    unsigned char   flags;
    unsigned char   signature;
    unsigned int    volumeId;
    unsigned char   volumeLabel[11];
    unsigned char   fileSystem[8];
} __attribute__((__packed__)) BiosParamBlock;

typedef struct DirEntry
{
    unsigned char   name[8];
    unsigned char   ext[3];
    unsigned char   attribs;
    unsigned char   reserved;
    unsigned char   createTimeMs;
    unsigned short  createTime;
    unsigned short  createDate;
    unsigned short  accessDate;
    unsigned short  extendedAttribsIndex;
    unsigned short  mTime;
    unsigned short  mDate;
    unsigned short  clusterIndex;
    unsigned int    fileSize;
} __attribute__((__packed__)) DirEntry;

unsigned int FatGetTotalSectorCount(unsigned char *image);
unsigned int FatGetMetaSectorCount(unsigned char *image);
unsigned int FatGetClusterCount(unsigned char *image);
unsigned int FatGetImageSize(unsigned char *image);
unsigned short *FatGetTable(unsigned char *image, unsigned int fatIndex);
unsigned short FatGetClusterValue(unsigned char *image, unsigned int fatIndex, unsigned int clusterIndex);
void FatSetClusterValue(unsigned char *image, unsigned int fatIndex, unsigned int clusterIndex, unsigned short value);
unsigned int FatGetClusterOffset(unsigned char *image, unsigned int clusterIndex);
DirEntry *FatGetRootDirectory(unsigned char *image);
unsigned char *FatAllocImage(unsigned int imageSize);
bool FatInitImage(unsigned char *image, unsigned char *bootSector);
void FatSplitPath(unsigned char dstName[8], unsigned char dstExt[3], const char *path);
unsigned short FatFindFreeCluster(unsigned char *image);
void FatUpdateCluster(unsigned char *image, unsigned int clusterIndex, unsigned short value);
DirEntry *FatFindFreeRootEntry(unsigned char *image);
void FatUpdateDirEntry(DirEntry *entry, unsigned short clusterIndex, const unsigned char name[8], const unsigned char ext[3], unsigned int fileSize);
void FatRemoveDirEntry(DirEntry *entry);
unsigned short FatAddData(unsigned char *image, const void *data, unsigned int len);
void FatRemoveData(unsigned char *image, unsigned int rootClusterIndex);
DirEntry *FatAddFile(unsigned char *image, const char *path, const void *data, unsigned int len);
void FatRemoveFile(unsigned char *image, DirEntry *entry);