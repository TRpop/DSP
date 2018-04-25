/* 
  * pxa27x GPS Example firmware program
  *    2009-05-28
  *    M-Tech Co.,Ltd.
  *    lee byung kwon
  */

#include <lib.h>
#include <serial.h>
#include <reg.h>

#define IO_GPIO112 (1<<16)

void gps_init(void)
{
    SerialInit_BT9600();

    // GPS Mode Settings
	*(unsigned int *)GPIO112_MFPR = 0xC800;
    __REG(GPIO_BASE + GPDR3) |= IO_GPIO112;
    printf("GPS Module Communication Mode **\n");
    __REG(GPIO_BASE + GPSR3) |=IO_GPIO112;
}

void c_main(void)
{
    char val,ch;

    gps_init();
    while (1 ) {
	if (BTGetChar(&val)==1){
	    printf("%c",val);
	}

	if (GetChar(&ch) ==1) {
	    if (ch == 'Q') break;
	    if (ch == 'q') break;
	}
    }
}
