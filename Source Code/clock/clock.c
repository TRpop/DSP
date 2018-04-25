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


// 날자 구조체
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

// 년, 월, 일 구조체
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




// CLOCK 프로그램 메인 
void c_main(void)
{

    	char ch;
	
   	// 구조체 변수 할당
    	DayRegister  dayReg;
    	YearRegister yearReg;
	char dow[][5]={"Sun.","Mon.","Tue.","Wed.","Thu.","Fri.","Sat."};	
	
    	// 레지스터 변수 초기 설정
    	yearReg.year        		= 2009;
	yearReg.month       	= 06;
	yearReg.dayOfMonth 	= 1;
	dayReg.dayOfWeek   	= 2;
	dayReg.hours        		= 13;
	dayReg.minutes      	= 30;
	dayReg.seconds      	= 0;


    	lcd_init();

   	// 년, 월, 일 시리얼 디스플레이
    	printf(" %d %d %d %s ",yearReg.year,yearReg.month,yearReg.dayOfMonth,dow[dayReg.dayOfWeek-1]); //
    	printf("\r\n                "); //


    	// 순서대로 써넣어야 한다.
    	RYCR = yearReg.value;
    	RDCR = dayReg.value;
	
    	// 주기적인 알람 1초
    	PIAR = 1000;
	
    	// 주기적인 인터럽트 인에이블
    	RTSR = RTSR_PICE | RTSR_PIALE;

	
    	// 무한 루프
    	while(1){
		
        	while(!(RTSR & RTSR_PIAL)); // 1초 대기
        	
        	dayReg.value = RDCR;
        	yearReg.value = RYCR;

    		// 년, 월, 일 디스플레이
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

    	RTSR |= RTSR_PIAL;   // PIAL 클리어

	// Clock Example Program quit 
    	if (GetChar(&ch) ==1) {
	    if (ch == 'Q') break;
	    if (ch == 'q') break;
	}
		
    }


}

