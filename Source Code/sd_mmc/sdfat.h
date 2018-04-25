//
//  Header: sdfat.h
//
#ifndef SDFAT_H
#define SDFAT_H

#include <serial.h>
#include <lib.h>
#include <reg.h>
#include <time.h>

#include "sdmdd.h"
#include "sdpdd.h"


#define TRUE 1
#define FALSE 0

// signal error for function which return DWORD value
#define FAT_ERROR ((DWORD)-1)
// disk function
BOOL  FATInitDisk();
DWORD FATGetDiskSize();
DWORD FATReadDisk(PVOID pBuffer, DWORD dwLength, DWORD dwPos);
// partition function
DWORD FATGetPartitionNumber();
DWORD FATGetPartitionSize(DWORD dwPartNo);
DWORD FATReadPartition(DWORD dwPartNo, PVOID pBuffer, DWORD dwLength, DWORD dwPos);
// file function
typedef struct _FILEINFO{ // keep track file property
    DWORD PartitionNumber;
    DWORD StartingCluster; // 0 means root
    DWORD FileLength;
    DWORD FileAttribute;
    DWORD CurrentCluster;
    DWORD CurrentPostion;
}FILEINFO, *PFILEINFO;

BOOL  FATOpenFile(PFILEINFO pFileInfo, PCSTR pFileName);
BOOL  FATCloseFile(PFILEINFO pFileInfo); // close file isn't necessary

BOOL  FATIsReadOnly(PFILEINFO pFileInfo);
BOOL  FATIsHidden(PFILEINFO pFileInfo);
BOOL  FATIsSystem(PFILEINFO pFileInfo);
BOOL  FATIsVolumeLabel(PFILEINFO pFileInfo);
BOOL  FATIsDirectory(PFILEINFO pFileInfo);
BOOL  FATIsArchive(PFILEINFO pFileInfo);

BOOL  FATEndOfFile(PFILEINFO pFileInfo);
DWORD FATGetFileSize(PFILEINFO pFileInfo);

DWORD FATGetFilePos(PFILEINFO pFileInfo);
BOOL  FATSetFilePos(PFILEINFO pFileInfo, DWORD dwPos);

DWORD FATReadFile(PFILEINFO pFileInfo, PVOID pBuffer, DWORD dwLength);
DWORD FATReadFileEx(PFILEINFO pFileInfo, PVOID pBuffer, DWORD dwLength, DWORD dwPos);

// dump function for testing only
void DumpMasterBootRecord();
void DumpPartitionBootSector(DWORD dwPartNo);
void DumpFATTable(DWORD dwPartNo, DWORD dwStart, DWORD dwCount);
void DumpDirectory(PCSTR pDirName, DWORD dwStart, DWORD dwCount);
void DumpDirectoryEx(PCSTR pDirName);

#endif // SDFAT_H
