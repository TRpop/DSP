/* 
  * pxa27x RTC Example firmware program
  *    2009-05-28
  *    M-Tech coperation.,ltd.
  *    lee byung kwon
  */

// Common Headers
#include <serial.h>
#include <lib.h>
#include <reg.h>
#include <time.h>
#include <types.h>

// Clock firmware Headers
#include "lcd.h"


// ���� ����ü
typedef union DAY_REGISTER 
{
    unsigned int value;
    struct
    {
        unsigned seconds    : 6;
        unsigned minutes    : 6;
        unsigned hours      : 5;
        unsigned dayOfWeek  : 3;
        unsigned weekOfMonth: 3;
        unsigned reserved   : 9;
    };
} DayRegister;

// ��, ��, �� ����ü
typedef union YEAR_REGISTER
{
    unsigned int value;
    struct 
    {
        unsigned dayOfMonth : 5;
        unsigned month      : 4;
        unsigned year       : 12;
        unsigned reserved   : 11;
    };
} YearRegister;




// CLOCK ���α׷� ���� 
void c_main(void)
{

    	char ch;
	
   	// ����ü ���� �Ҵ�
    	DayRegister  dayReg;
    	YearRegister yearReg;
	char dow[][5]={"Sun.","Mon.","Tue.","Wed.","Thu.","Fri.","Sat."};	
	
    	// �������� ���� �ʱ� ����
    	yearReg.year        		= 2009;
	yearReg.month       	= 06;
	yearReg.dayOfMonth 	= 1;
	dayReg.dayOfWeek   	= 2;
	dayReg.hours        		= 13;
	dayReg.minutes      	= 30;
	dayReg.seconds      	= 0;


    	lcd_init();

   	// ��, ��, �� �ø��� ���÷���
    	printf(" %d %d %d %s ",yearReg.year,yearReg.month,yearReg.dayOfMonth,dow[dayReg.dayOfWeek-1]); //
    	printf("\r\n                "); //


    	// ������� ��־�� �Ѵ�.
    	RYCR = yearReg.value;
    	RDCR = dayReg.value;
	
    	// �ֱ����� �˶� 1��
    	PIAR = 1000;
	
    	// �ֱ����� ���ͷ�Ʈ �ο��̺�
    	RTSR = RTSR_PICE | RTSR_PIALE;

	
    	// ���� ����
    	while(1){
		
        	while(!(RTSR & RTSR_PIAL)); // 1�� ���
        	
        	dayReg.value = RDCR;
        	yearReg.value = RYCR;

    		// ��, ��, �� ���÷���
    		printCharLCD("\r %d %02d %02d %s\n",yearReg.year,yearReg.month,yearReg.dayOfMonth,dow[dayReg.dayOfWeek-1]); //
		
	        // display & print
	        if(dayReg.hours > 12){
	            printCharLCD(" PM : %d.%d.%02d",dayReg.hours-12,dayReg.minutes,dayReg.seconds);
	            printf("\r PM : %d.%d.%02d",dayReg.hours-12,dayReg.minutes,dayReg.seconds);			
	        }
	        else{
	            printCharLCD(" AM : %d.%d.%02d",dayReg.hours,dayReg.minutes,dayReg.seconds);
	            printf("\r  AM : %d.%d.%02d",dayReg.hours,dayReg.minutes,dayReg.seconds);			
	        }

    	RTSR |= RTSR_PIAL;   // PIAL Ŭ����

	// Clock Example Program quit 
    	if (GetChar(&ch) ==1) {
	    if (ch == 'Q') break;
	    if (ch == 'q') break;
	}
		
    }


}

