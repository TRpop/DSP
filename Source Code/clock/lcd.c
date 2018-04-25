//------------------------------------------------------------------------------
// 화일명 : lcd.c
// 설  명 : Text LCD 제어
//
//
//------------------------------------------------------------------------------

//******************************************************************************
// 헤더 정의
//******************************************************************************
#include <lib.h>
#include <time.h>
#include "lcd.h"

#define ADDR_OF_LCD		0x14809000

//******************************************************************************
// 전역변수 정의
//******************************************************************************
unsigned short *lcd_addr;


//Text LCD를 제어와 관련
#define CTRL    0
#define WRITE   1
#define READ    2

void LCD_word(short Control, short Data)
{
	__REG16(ADDR_OF_LCD) = Data | 0x0000;
	Wait(300);
}

////////////////////////////////////////////////////////////////
// Char LCD로 디스플레이
// 문자열을 TFTLCD로 디스플레이
void printCharLCD(const char *fmt, ...)
{
    	va_list   v1;
    	unsigned int  count=0;
    	unsigned char buffer[100];
    	va_start(v1,fmt);
    	count = vsprintf(buffer,fmt,v1);
    	va_end(v1);
    	buffer[count] = '\0';
    	OutputLcd(buffer);

}

// 문자열을 char LCD로 디스플레이
void OutputLcd(unsigned char *str)
{

	int i, len;
	unsigned char data;
	unsigned short ldata, wr_data, we_data ;

	len = StrLen(str);

	for(i=0;i<len;i++) {
		
		data = *(str+i);

		if(data == '\r') {
			LCD_word(CTRL,0x0101); // allclr
			LCD_word(CTRL,0x0001);
		}
		else if(data == '\n') {
			LCD_word(CTRL, 0x01C0);
			LCD_word(CTRL, 0x00C0);
			i++;	
		   }

		ldata = (short)*(str+i);
		wr_data = ldata + 0x0500;
		we_data = ldata + 0x0400;
		
		LCD_word(WRITE,wr_data);
		LCD_word(WRITE,we_data);
	}

}



void lcd_init(void)
{

	LCD_word(CTRL,0x0138);  // function set
	LCD_word(CTRL,0x0038);

	LCD_word(CTRL,0x010E); // display on
	LCD_word(CTRL,0x000E);

	LCD_word(CTRL,0x0101); // allclr
	LCD_word(CTRL,0x0001);

	LCD_word(CTRL,0x0102); // Ln11
	LCD_word(CTRL,0x0002);

	LCD_word(CTRL,0x0106);
	LCD_word(CTRL,0x0006); 

}

