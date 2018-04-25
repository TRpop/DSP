/* 
  * TD242LP Access Example firmware program
  *    2009-05-28
  *    M-Tech coperation.,ltd.
  *    lee byung kwon
  */
#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#define TD242_BASE  0x14200000


void c_main(void)
{

int TD242LP_c_main(void);
   char ch;

     while(1) {

      if(GetChar(&ch) ==1){

     switch(ch) {

     case '0':
		TD242_RegDump();
		break;
     case '1':
		TD242_MemTest();
		break;

     default:
	 	TD242LP_Menu_message(1);
      }


     if (ch == 'q') break;
     if (ch == 'Q') break;
      	} 

   }

}


int TD242_RegDump(void)
{

	unsigned short *pLastAddr;
	unsigned short *pAttrAddr;

	pAttrAddr =  (unsigned short *)TD242_BASE;


	printf("--------------------------------------------\n");	
	printf("Top Level reg\t|00|\t|04|\t|08|\t|0C|\n");
	printf("--------------------------------------------\n");	
	for(pLastAddr=pAttrAddr+0x001f;pAttrAddr<pLastAddr;pAttrAddr+=8)
	{
	  printf("0x%x : \t%04x \t%04x \t%04x \t%04x \r\n",pAttrAddr,*pAttrAddr, *(pAttrAddr+2),
	  	 *(pAttrAddr+4), *(pAttrAddr+6));
	}
	printf("--------------------------------------------\n");	
	printf("Function reg \t|00|\t|04|\t|08|\t|0C|\n");
	printf("--------------------------------------------\n");
	for(pLastAddr=pAttrAddr+0x001f;pAttrAddr<pLastAddr;pAttrAddr+=8)
	{
	  printf("0x%x : \t%04x \t%04x \t%04x \t%04x \r\n",pAttrAddr,*pAttrAddr, *(pAttrAddr+2),
	  	 *(pAttrAddr+4), *(pAttrAddr+6));
	}
	printf("--------------------------------------------\n");	
	printf("Host reg  \t|00|\t|04|\t|08|\t|0C|\n");
	printf("--------------------------------------------\n");
	for(pLastAddr=pAttrAddr+0x003e;pAttrAddr<pLastAddr;pAttrAddr+=8)
	{
	  printf("0x%x : \t%04x \t%04x \t%04x \t%04x \r\n",pAttrAddr,*pAttrAddr, *(pAttrAddr+2),
	  	 *(pAttrAddr+4), *(pAttrAddr+6));
	}

	return 0;
}

int TD242_MemTest(void)
{
	unsigned short *pLastAddr;
	unsigned short *pAttrAddr;

	pAttrAddr =  (unsigned short *)(TD242_BASE + 0x200);
	for(pLastAddr=pAttrAddr+0x001f;pAttrAddr<pLastAddr;pAttrAddr+=8)
	{
	  printf("0x%x : \t%04x \t%04x \t%04x \t%04x \r\n",pAttrAddr,*pAttrAddr, *(pAttrAddr+2),
	  	 *(pAttrAddr+4), *(pAttrAddr+6));
	}

	
	pAttrAddr =  (unsigned short *)(TD242_BASE + 0x1000);
	
	for(pLastAddr=pAttrAddr+0x001f;pAttrAddr<pLastAddr;pAttrAddr+=8)
	{
	  printf("0x%x : \t%04x \t%04x \t%04x \t%04x \r\n",pAttrAddr,*pAttrAddr, *(pAttrAddr+2),
	  	 *(pAttrAddr+4), *(pAttrAddr+6));
	}

	return 0;
}

int TD242LP_Menu_message(int number)
{

	switch(number) {
	
	case 1 :
		printf("################ MENU ###############\n");
		printf(" [0] Register Dump\n");
		printf(" [1] DATA Memory test\n");
		printf(" [Q] quit\n");
		printf(" Press any key to Card check \n");
		printf("###################################\n\n");
		break;
	
	default :
			break;
	}
	return 0;	
}


