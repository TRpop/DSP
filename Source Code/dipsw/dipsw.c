#include<lib.h>
#include <time.h>

#define ADDR_OF_LED		0x14805000   /* FPGA LEDs */
#define ADDR_OF_DIPA		0x14803000   /* FPGA Dip Switch A */
#define ADDR_OF_DIPB		0x14804000   /* FPGA Dip Switch B */


void  c_main(void)
{
	int i;
	unsigned char * led_addr;
	unsigned char * dipA_addr;
	unsigned char * dipB_addr;
	unsigned char dipA_value, dipB_value;

	/* �����ּ� �Ҵ� */
	dipA_addr = (unsigned char *)ADDR_OF_DIPA;
	dipB_addr = (unsigned char *)ADDR_OF_DIPB;
	led_addr = (unsigned char *)ADDR_OF_LED;

	*led_addr= 0x00;

	/* Dip Switch A �� B �� ������ ���鼭 100ȸ ��� */
	for ( i=0; i<100; i++) {	

		dipA_value = *dipA_addr;    /* Dip Switch A �ּҸ� �о� dipA_value �� ���� */
		dipB_value = *dipB_addr;    /* Dip Switch B �ּҸ� �о� dipB_value �� ���� */

		*led_addr= dipA_value;
		msWait(100);
		*led_addr= dipB_value;
		msWait(100);		
	}
	
	*led_addr= 0x00;


}
