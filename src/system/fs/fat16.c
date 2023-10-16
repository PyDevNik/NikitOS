#include <fat16.h>
#include <memory.h>
#include <str.h>
#include <stdbool.h>
#include <types.h>

unsigned int FatGetTotalSectorCount(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    if (bpb->sectorCount) {return bpb->sectorCount;}
    else {return bpb->largeSectorCount;}
}

unsigned int FatGetMetaSectorCount(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    return
        bpb->reservedSectorCount +
        bpb->fatCount * bpb->sectorsPerFat +
        (bpb->rootEntryCount * sizeof(DirEntry)) / bpb->bytesPerSector;
}

unsigned int FatGetClusterCount(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    unsigned int totalSectorCount = FatGetTotalSectorCount(image);
    unsigned int metaSectorCount = FatGetMetaSectorCount(image);
    unsigned int dataSectorCount = totalSectorCount - metaSectorCount;
    return dataSectorCount / bpb->sectorsPerCluster;
}

unsigned int FatGetImageSize(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    return FatGetTotalSectorCount(image) * bpb->bytesPerSector;
}

unsigned short *FatGetTable(unsigned char *image, unsigned int fatIndex)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    unsigned int offset = (bpb->reservedSectorCount + fatIndex * bpb->sectorsPerFat) * bpb->bytesPerSector;
    return (unsigned short *)(image + offset);
}

unsigned short FatGetClusterValue(unsigned char *image, unsigned int fatIndex, unsigned int clusterIndex)
{
    unsigned short *fat = FatGetTable(image, fatIndex);
    return fat[clusterIndex];
}

unsigned int FatGetClusterOffset(unsigned char *image, unsigned int clusterIndex)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    return
        (bpb->reservedSectorCount + bpb->fatCount * bpb->sectorsPerFat) * bpb->bytesPerSector +
        bpb->rootEntryCount * sizeof(DirEntry) +
        (clusterIndex - 2) * (bpb->sectorsPerCluster * bpb->bytesPerSector);
}

void FatSetClusterValue(unsigned char *image, unsigned int fatIndex, unsigned int clusterIndex, unsigned short value)
{
    unsigned short *fat = FatGetTable(image, fatIndex);
    fat[clusterIndex] = value;
}

DirEntry *FatGetRootDirectory(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    unsigned int offset = (bpb->reservedSectorCount + bpb->fatCount * bpb->sectorsPerFat) * bpb->bytesPerSector;
    unsigned int dataSize = bpb->rootEntryCount * sizeof(DirEntry);
    return (DirEntry *)(image + offset);
}

unsigned char *FatAllocImage(unsigned int imageSize)
{
    unsigned char *image = (unsigned char *)kmalloc(imageSize);
    memset(image, ENTRY_ERASED, imageSize);
    return image;
}

bool FatInitImage(unsigned char *image, unsigned char *bootSector)
{
    BiosParamBlock *bpb = (BiosParamBlock *)bootSector;
    if (bootSector[0x1fe] != 0x55 || bootSector[0x1ff] != 0xaa) {return FALSE;}
    mecpy(image, bootSector, bpb->bytesPerSector);
    unsigned int clusterCount = FatGetClusterCount(image);
    FatUpdateCluster(image, 0, 0xff00 | bpb->mediaType);    
    FatUpdateCluster(image, 1, 0xffff);                     
    for (unsigned int clusterIndex = 2; clusterIndex < clusterCount; ++clusterIndex) {FatUpdateCluster(image, clusterIndex, 0x0000);}
    return TRUE;
}

unsigned short FatFindFreeCluster(unsigned char *image)
{
    unsigned int clusterCount = FatGetClusterCount(image);
    unsigned short *fat = FatGetTable(image, 0);
    for (unsigned int clusterIndex = 2; clusterIndex < clusterCount; ++clusterIndex)
    {
        unsigned short data = fat[clusterIndex];
        if (data == 0) {return clusterIndex;}
    }

    return 0;
}

void FatUpdateCluster(unsigned char *image, unsigned int clusterIndex, unsigned short value)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    for (unsigned int fatIndex = 0; fatIndex < bpb->fatCount; ++fatIndex) {FatSetClusterValue(image, fatIndex, clusterIndex, value);}
}

DirEntry *FatFindFreeRootEntry(unsigned char *image)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    DirEntry *start = FatGetRootDirectory(image);
    DirEntry *end = start + bpb->rootEntryCount;
    for (DirEntry *entry = start; entry != end; ++entry)
    {
        unsigned char marker = entry->name[0];
        if (marker == ENTRY_AVAILABLE || marker == ENTRY_ERASED) {return entry;}
    }
    return 0;
}

static void SetPaddedString(unsigned char *dst, unsigned int dstLen, const char *src, unsigned int srcLen)
{
    if (src)
    {
        if (srcLen > dstLen) {mecpy(dst, src, dstLen);}
        else {mecpy(dst, src, srcLen); memset(dst + srcLen, ' ', dstLen - srcLen);}
        for (unsigned int i = 0; i < dstLen; ++i) {dst[i] = toupper(dst[i]);}
    }
    else {memset(dst, ' ', dstLen);}
}

void FatSplitPath(unsigned char dstName[8], unsigned char dstExt[3], const char *path)
{
    const char *name = strrchr(path, '/');
    if (name) {name = name + 1;}
    else {name = path;}
    unsigned int nameLen = strlen(name);
    char *ext = 0;
    unsigned int extLen = 0;
    char *p = strchr(name, '.');
    if (p) {nameLen = p - name; ext = p + 1; extLen = strlen(ext);}
    SetPaddedString(dstName, 8, name, nameLen);
    SetPaddedString(dstExt, 3, ext, extLen);
}

void FatUpdateDirEntry(DirEntry *entry, unsigned short clusterIndex, const unsigned char name[8], const unsigned char ext[3], unsigned int fileSize)
{
    entry->clusterIndex = clusterIndex;
    mecpy(entry->name, name, sizeof(entry->name));
    mecpy(entry->ext, ext, sizeof(entry->ext));
    entry->fileSize = fileSize;
}

void FatRemoveDirEntry(DirEntry *entry) {entry->name[0] = ENTRY_AVAILABLE;}

unsigned short FatAddData(unsigned char *image, const void *data, unsigned int len)
{
    BiosParamBlock *bpb = (BiosParamBlock *)image;
    unsigned int bytesPerCluster = bpb->sectorsPerCluster * bpb->bytesPerSector;
    if (len == 0) {return 0;}
    unsigned short endOfChainValue = FatGetClusterValue(image, 0, 1);
    unsigned short prevClusterIndex = 0;
    unsigned short rootClusterIndex = 0;
    const unsigned char *p = (const unsigned char *)data;
    const unsigned char *end = p + len;
    while (p < end)
    {   
        unsigned short clusterIndex = FatFindFreeCluster(image);
        if (clusterIndex == 0)
        {   
            if (rootClusterIndex != 0) {FatRemoveData(image, rootClusterIndex);}
            return 0;
        }
        unsigned int count = end - p;
        if (count > bytesPerCluster) {count = bytesPerCluster;}
        unsigned int offset = FatGetClusterOffset(image, clusterIndex);
        memcpy(image + offset, p, count);
        p += count;
        FatUpdateCluster(image, clusterIndex, endOfChainValue);
        if (prevClusterIndex) {FatUpdateCluster(image, prevClusterIndex, clusterIndex);}
        else {rootClusterIndex = clusterIndex;}
        prevClusterIndex = clusterIndex;
    }
    return rootClusterIndex;
}

void FatRemoveData(unsigned char *image, unsigned int clusterIndex)
{
    unsigned short endOfChainValue = FatGetClusterValue(image, 0, 1);
    while (clusterIndex != endOfChainValue)
    {
        unsigned short nextClusterIndex = FatGetClusterValue(image, 0, clusterIndex);
        FatUpdateCluster(image, clusterIndex, 0);
        clusterIndex = nextClusterIndex;
    }
}

DirEntry *FatAddFile(unsigned char *image, const char *path, const void *data, unsigned int len)
{
    DirEntry *entry = FatFindFreeRootEntry(image);
    if (!entry) {return 0;}
    unsigned short rootClusterIndex = FatAddData(image, data, len);
    if (!rootClusterIndex) {return 0;}
    unsigned char name[8];
    unsigned char ext[3];
    FatSplitPath(name, ext, path);
    FatUpdateDirEntry(entry, rootClusterIndex, name, ext, len);
    return entry;
}

void FatRemoveFile(unsigned char *image, DirEntry *entry) {FatRemoveData(image, entry->clusterIndex); FatRemoveDirEntry(entry);}