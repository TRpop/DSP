#include<lib.h>
#include <time.h>

#define ADDR_OF_Buzzer		0x14808000   /* FPGA LEDs */


void c_main(void)
{
	int i;
	unsigned char * buzzer_addr;
	
	buzzer_addr = (unsigned char *)ADDR_OF_Buzzer;
	* buzzer_addr= 0x00;

	/* 부저음을 100번 울린다 */
	for ( i=0; i<10; i++) {
		* buzzer_addr= 0x01;
		msWait(20);
		* buzzer_addr= 0x00;
		msWait(20);
	}
}
