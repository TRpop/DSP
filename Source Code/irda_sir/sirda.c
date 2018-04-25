/* 
  * pxa27x IrDA Example firmware program
  *    2009-05-29
  *    M-Tech coperation.,ltd.
  *    lee byung kwon
  */

#include <serial.h>
#include <lib.h>
#include <reg.h>
#include <time.h>


void Sir_Init(void);
void Sir_PutChar(const char ch);
char Sir_GetChar(char *ch);

#define IO_GPIO108 (1<<12)
#define IO_GPIO107 (1<<11)

#define ICP_RXD 	IO_GPIO108
#define ICP_TXD	IO_GPIO107

#define ALT_ICP_RXD (1<<24)	// GP108, Alt 01
#define ALT_ICP_TXD (1<<22)	// GP107, Alt 01
#define CLKEN_ICP	(1<<15)	// Infrared Port Clock Enable

#define RATE_115200 (0x00000008)
#define RATE_38400  (0x00000018)
#define RATE_9600   (0x00000060)



void Sir_Init(void)
{


	// ST_UART Init
	__REG(STUART_BASE + STLCR) = 0;	// DLAB = 0
	__REG(STUART_BASE + STDLH) = 0;	// IER_DLH = 0
	// Baudrate = 115,200 bps
	__REG(STUART_BASE + STLCR) = 0x80;
	__REG(STUART_BASE + STDLL) = RATE_115200;
	__REG(STUART_BASE + STDLH) = 0x00;
	__REG(STUART_BASE + STLCR) = 0x00;	// DLAB = 0
	// 8Bit, Non parity, Stop 1 bit
	__REG(STUART_BASE + STLCR) = 0x03;
	__REG(STUART_BASE + STFCR) = 0x01;	// FIFO Enable
	__REG(STUART_BASE + STFCR) = 0x07;	// Reset TX,RX FIFO clean, FIFO Enable
	// UART Polling mode 
	__REG(STUART_BASE + STDLH) = 0x00;
	__REG(STUART_BASE + STISR) = 0x01;	// Infrared Mode
	__REG(STUART_BASE + STMCR) = 0x10;	// Loopback Mode
	// GPIO Pin setting
	*(unsigned int *)GPIO108_MFPR = 0xC801;
	*(unsigned int *)GPIO107_MFPR = 0xC801;
    	__REG(GPIO_BASE + GPCR3) |= ICP_TXD;
    	__REG(GPIO_BASE + GPDR3) &= ~ICP_RXD;
	__REG(GPIO_BASE + GPDR3) |= ICP_TXD;	

	// Start ICP Clock
	CKENA |= CLKEN_ICP;
	__REG(STUART_BASE + STDLH) = 0x40;
	
}

void Sir_PutChar(const char ch)
{
	while ((__REG(STUART_BASE + STLSR) & 0x00000020) == 0 );	
	__REG(STUART_BASE + STTHR) = ((u32)ch & 0xFF);
	if (ch=='\n') Sir_PutChar('\r');	
}


char Sir_GetChar(char *ch)
{
	if((__REG(STUART_BASE + STISR) & 0x01)==0){
		return 0;
	} else {
		*(volatile char *)ch = __REG(STUART_BASE + STRBR);
		return 1;
	}	
}


//
// IrDA Example main routine
//
int c_main(void)
{
    char val,ch;

    Sir_Init();

    printf("IrDA Chatting. Quit[Q] \n");
	
    while (1 ) {
	if (Sir_GetChar(&val)==1){
	    printf("%c",val);
	}

	if (GetChar(&ch) ==1) {

	   // User input data send to IrDA
   	   Sir_PutChar(ch);
	   // Echo user input data 
	   printf("%c",ch);
	   
	   if (ch == 'Q') break;
	   if (ch == 'q') break;
	}
    }
    return 0;
}

