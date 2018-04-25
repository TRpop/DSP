#include <lib.h>
#include <serial.h>
#include <time.h>


#define ADDR_OF_7SEG_DATA   0x14801000
#define ADDR_OF_7SEG_GRID   0x14800000

int Getsegcode(int x);


void c_main(void)
{
	int i,j;
	char ch;
	unsigned int count=0,temp1,temp2;
	char data[6];
	unsigned int digit[6] ={0x20,0x10,0x08,0x04,0x02,0x01};
	unsigned char * seg_addr_grid, * seg_addr_data;

	// clear sed
	seg_addr_grid = (unsigned char *)ADDR_OF_7SEG_GRID;
	seg_addr_data = (unsigned char *)ADDR_OF_7SEG_DATA;

	*seg_addr_grid = 0x00;
	*seg_addr_data = 0x00;

	while(count<999999) {
		if(GetChar(&ch)==1) {
			break;
		}

		data[5]=Getsegcode(count/100000);
		temp1=count%100000;
		data[4]=Getsegcode(temp1/10000);
		temp2=temp1%10000;
		data[3]=Getsegcode(temp2/1000);
		temp1=temp2%1000;
		data[2]=Getsegcode(temp1/100);
		temp2=temp1%100;
		data[1]=Getsegcode(temp2/10);
		data[0]=Getsegcode(temp2%10);
		for(j=0;j<100;j++) {
			for(i=0;i<6;i++)
			{
				*seg_addr_grid = ~digit[i];
				*seg_addr_data = data[i];
				Wait(50);    // 50us delay
			}
		}
		count++;
	}
	// clear sed
	*seg_addr_grid = 0x00;
	*seg_addr_data = 0x00;

}

   
int Getsegcode(int x)
{
	char code;
	switch (x) {
		case 0 : code = 0x3f; break;
		case 1 : code = 0x06; break;
		case 2 : code = 0x5b; break;
		case 3 : code = 0x4f; break;
		case 4 : code = 0x66; break;
		case 5 : code = 0x6d; break;
		case 6 : code = 0x7d; break;
		case 7 : code = 0x07; break;
		case 8 : code = 0x7f; break;
		case 9 : code = 0x6f; break;
		default  : code = 0;    break;
	}
	return code;
}

