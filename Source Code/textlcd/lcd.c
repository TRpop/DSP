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

#define ADDR_OF_LCD		0x14809000		// PXA 27x Based


//******************************************************************************
// 전역변수 정의
//******************************************************************************
unsigned short *lcd_addr;

//******************************************************************************
// 함수 정의
//******************************************************************************
void setcommand(unsigned short command);
void writebyte(char ch);
void initialize_textlcd();
void write_string(char *str,int length);
int function_set(int rows, int nfonts);
int display_control(int display_enable, int cursor_enable, int nblink);
int cusrsor_shit(int set_screen, int set_rightshit);
int entry_mode_set(int increase, int nshift);
int return_home();
int clear_display();
int set_ddram_address(int pos);


void setcommand(unsigned short command)
{

    *lcd_addr = command | 0x0100;
      Wait(300);    	// 300us
    *lcd_addr = command | 0x0000;
      Wait(300);    	// 300us
}

void writebyte(char ch)
{
    unsigned short data;
    data = ch & 0x00FF;

    *lcd_addr = data|0x500;   // dvk270peri  .eq. bulverde
      Wait(300);    	// 300us

    *lcd_addr = data|0x400;
      Wait(300);    	// 300us
}

int function_set(int rows, int nfonts){
    unsigned short command = 0x30;

    if(rows == 2) command |= 0x08;
    else if(rows == 1) command &= 0xf7;
    else return -1;

    command = nfonts ? (command | 0x04) : command;
    setcommand(command);
    return 1;
}

int display_control(int display_enable, int cursor_enable, int nblink){
    unsigned short command = 0x08;
    command = display_enable ? (command | 0x04) : command;
    command = cursor_enable ? (command | 0x02) : command;
    command = nblink ? (command | 0x01) : command;
    setcommand(command);
    return 1;
}

int cusrsor_shit(int set_screen, int set_rightshit){
    unsigned short command = 0x10;
    command = set_screen ? (command | 0x08) : command;
    command = set_rightshit ? (command | 0x04) : command;
    setcommand(command);
    return 1;
}

int entry_mode_set(int increase, int nshift){
    unsigned short command = 0x04;
    command = increase ? (command | 0x2) : command;
    command = nshift ? ( command | 0x1) : command;
    setcommand(command);
    return 1;
}

int return_home(){
    unsigned short command = 0x02;
    setcommand(command);
    Wait(300);    	// 300us
    return 1;
}

int clear_display(){
    unsigned short command = 0x01;
    setcommand(command);
    Wait(300);    	// 300us
    return 1;
}

int set_ddram_address(int pos){
    unsigned short command = 0x80;
    command += pos;
    setcommand(command);
    return 1;
}

void initialize_textlcd(){
    function_set(2,0); //Function Set:8bit,display 2lines,5x7 mod
    display_control(1,0,0); // Display on, Cursor off
    clear_display(); // Display clear
    return_home(); // go home
    entry_mode_set(1,0); // Entry Mode Set : shift right cursor
    Wait(300);    	// 300us
}


void c_main(void)
{
    int i;
    char line1[16];
    char line2[16];
    lcd_addr = (unsigned short *)ADDR_OF_LCD;
    line1[0]='W';line1[1]='e';line1[2]='l';line1[3]='c';
    line1[4]='o';line1[5]='m';line1[6]='e';line1[7]=' ';
    line1[8]='t';line1[9]='o';line1[10]=' ';line1[11]=' ';
    line1[12]=' ';line1[13]=' ';line1[14]=' ';line1[15]=' ';
    line2[0]='E';line2[1]='m';line2[2]='b';line2[3]='e';
    line2[4]='d';line2[5]='d';line2[6]='e';line2[7]='d';
    line2[8]=' ';line2[9]='w';line2[10]='o';line2[11]='r';
    line2[12]='l';line2[13]='d';line2[14]=' ';line2[15]=' ';

    initialize_textlcd();
    Wait(500);    	// 300us
    for(i=0;i<16;i++) writebyte(line1[i]);
    set_ddram_address(0x40);
    for(i=0;i<16;i++) writebyte(line2[i]);

}

