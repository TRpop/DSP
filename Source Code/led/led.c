#include <lib.h>
#include <time.h>

#define ADDR_OF_LED		0x14805000   /* FPGA LEDs */

void c_main(void)
{
	int i;
	unsigned char * led_addr;
	unsigned char mask;
	unsigned char blink_count=8;
	// clear led
	led_addr = (unsigned char *)ADDR_OF_LED;

	*led_addr= 0x00;

	mask=0x01;
	printf("move right to left\n");
	// move right to left
	for ( i=0; i<blink_count; i++) {
		*led_addr= mask;
		msWait(100); 
		mask <<= 1;
	}

	// clear led
	*led_addr= 0x00;
	mask = 0x80;
	printf("move left to right...\n");
	// move left to right
	for ( i=0; i<blink_count; i++) {
		*led_addr= mask;
		msWait(100); 
		mask >>= 1;
	}
	// clear led
	*led_addr = 0x00;

}
