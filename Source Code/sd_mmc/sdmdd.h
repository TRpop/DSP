//
//  Header: sdmdd.h
//
#ifndef __SDMDD_H
#define __SDMDD_H

#include <time.h>
#include <lib.h>
#include <serial.h>
#include <reg.h>
#include <types.h>

//----------------------------------------------------------------------------------
//
//			LIST OF MDD EXPORT FUNCTION
//
//----------------------------------------------------------------------------------

BOOL SDInitializeHardware();		// Call-through to PDD
BOOL SDReadDataBlock(PVOID pBuffer, DWORD dwLength, DWORD dwAddress);

#endif

