/* 
  * pxa27x SD/MMC Test  Example firmware program
  *    2009-05-30
  *    M-Tech coperation.,ltd.
  *    lee byung kwon
  */
#include <serial.h>
#include <lib.h>
#include <reg.h>
#include <time.h>

#include "sdmdd.h"
#include "sdpdd.h"
#include "sdfat.h"


static void sdmmc_message(void);

void c_main(void)
{

	char ch;

	printf("\r\n SD/MMC Test...\n");

	// FAT 초기 설정
	if(!FATInitDisk()){
		printf("ERROR : FAT initial Disk error !! \n\n");
		goto end_cmain;
	}

	while (1 ) {
		if (GetChar(&ch) ==1) {
		
		switch(ch){
			case '0' :
				DumpMasterBootRecord();
				break;
			case '1' :
				DumpPartitionBootSector(0);
				break;
			case '2' :
				DumpFATTable(0, 0, 16);
				break;
			case '3' :
				DumpDirectory("",0,16);
				break;
		}
		
		sdmmc_message();
		
	   	if (ch == 'Q') break;
	   	if (ch == 'q') break;
	}
    }

end_cmain:
	printf("Quit \n");
}


static void sdmmc_message(void)
{

	printf("\r\n======== SD/MMC Test Menu ===========\n");
	printf("    [0] Dump SD/MMC BootRecord\n");
	printf("    [1] Dump SD/MMC PartitionBootSector\n");
	printf("    [2] Dump SD/MMC FAT Table\n");	
	printf("    [3] Dump SD/MMC Directory\n");		
	printf("    [Q] Quit !!\n");
	printf("===================================\n");
}


