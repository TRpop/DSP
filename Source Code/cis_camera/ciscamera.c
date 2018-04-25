#include <types.h>
#include <reg.h>
#include <serial.h>
#include <lib.h>

static unsigned short *pSRAM;
volatile unsigned short * pfbdata;
volatile unsigned short * modeset;

#define CIS_1300K_SRAM_ADDRESS		0x16900000
#define CIS_1300K_IRQ_ENABLE			0x16980060
#define CIS_1300K_MODE_ADDRESS		0x16980066
#define FLB_ADD	0x80600400
#define FLB(x)	(*(volatile unsigned long*)(FLB_ADD + x)) 	/* Frame Buffer */

#define CIS_IMAGE_WIDTH		320
#define CIS_IMAGE_HEIGHT	240
#define CIS_VAL_SET		0x00
#define PP_ROW  		320
#define PP_COL  240


static unsigned int read_st=0;

/* 1.3M CMOS Camera Test[FPGA Path]  */
void c_main(void)
{
        int i,j,k;
        int flag;
        char ch;

        pSRAM =(unsigned short *)CIS_1300K_SRAM_ADDRESS;
        pfbdata =(unsigned short *)FLB_ADD;
	modeset= (unsigned short *)CIS_1300K_MODE_ADDRESS;
	
	*modeset= CIS_VAL_SET;

	*(unsigned int *)GPIO58_MFPR = 0xC800;
	__REG(GPIO_BASE+GPDR1) =	__REG(GPIO_BASE+GPDR1) | ~(1<<26);  //GP58, P320
        
        __REG16(CIS_1300K_IRQ_ENABLE) = 0x01;

        printf("Press 'q' or 'Q' key to exit...\n");
		
        while(1) {
			
                flag = (__REG(GPIO_BASE+GPLR1) & 0x04000000) >> 26;  //GPIO[58] // P320
                
                if(flag==0) read_st=0;

                if(flag==1 && read_st==0) {
                        read_st=1;

                        for(i=0;i<CIS_IMAGE_HEIGHT;i++) {
                                for(j=0;j<CIS_IMAGE_WIDTH;j++) {
                                        k = i *CIS_IMAGE_WIDTH +j;
                                        *(pfbdata+(i*PP_ROW)+j) = *(pSRAM+k);
                                }
                        }
                }
				
                if(GetChar(&ch) == 1) {
                        if (ch == 'Q') break;
                        if (ch == 'q') break;
                }
        }
        __REG16(CIS_1300K_IRQ_ENABLE) = 0x00;

}

