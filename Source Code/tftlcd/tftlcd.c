/**************************************
 *  PXA2xx LCD Cotroller Initial      
 *
 **************************************/

#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#include "tftlcd.h"

/* global */
int color_depth = 16;
int fg_color;
int bg_color;
int col;
int row;

unsigned long fb_offset;
volatile unsigned short * pfbdata;

#define BG_DEFAULT	1
#define FG_DEFAULT	0x00e000e0

#define FB_SIZE(x)	(((PP_COL * PP_ROW) * x) / 8)
#define ROW_SIZE(x)	((PP_ROW * x) / 8)


const unsigned short color_pattern[] = {
    _RED,
    _GREEN,		
    _BLUE,
    _CYAN,			
    _MAGENTA,		
    _YELLOW,		
    _GRAY,
    _WHITE,		
    _BLACK,		
};


void Background_Display_Init(void)
{
    int i;

    fg_color = FG_DEFAULT;
    bg_color = BG_DEFAULT;
    row = 0;
    col = 0;
    fb_offset = 0x00;

    for(i=0;i<(PP_COL * PP_ROW)*2; i+=2){
	FLB(i) = bg_color;
    }
}

void Glcd_Drew_FBox(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor)
{
    unsigned int i,j;

    pfbdata =(unsigned short*)FLB_ADD;

    for(i=Gy_st; i<Gy_end; i++)				// ypoint = 0 to 480
    {
	fb_offset=i*PP_ROW;						// offset = y * (0~640)
	//		fb_offset=i*640;						// offset = y * (0~640)

	for(j=Gx_st; j<Gx_end; j++)			// xpoint ++
	{
	    *(pfbdata+fb_offset+j) = Fcolor;
	}
    }

}

void Glcd_Drew_Box(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor)
{
    unsigned int j;

    pfbdata =(unsigned short*)FLB_ADD;

    fb_offset=Gy_st*PP_ROW;
    for(j=Gx_st; j<Gx_end; j++)	*(pfbdata+fb_offset+j) = Fcolor;

    fb_offset=Gy_end*PP_ROW;
    for(j=Gx_st; j<Gx_end; j++)	*(pfbdata+fb_offset+j) = Fcolor;

    for(j=Gy_st; j<Gy_end; j++)	
    {
	*(pfbdata+j*PP_ROW+Gx_st) = Fcolor;
	*(pfbdata+j*PP_ROW+Gx_end) = Fcolor;

    }
}

void Color_Bar_Display(void)
{
    printf("Color Bar Display..\n");
    Background_Display_Init();
    Glcd_Drew_FBox(1,1,LCD_HEIGHT,LCD_WIDTH/7,_GRAY);
    Glcd_Drew_FBox(0,LCD_WIDTH/7+0.5,LCD_HEIGHT,(LCD_WIDTH/7)*2+0.5,_YELLOW);
    Glcd_Drew_FBox(0,(LCD_WIDTH/7)*2+0.5,LCD_HEIGHT,(LCD_WIDTH/7)*3+0.5,_CYAN);
    Glcd_Drew_FBox(0,(LCD_WIDTH/7)*3+0.5,LCD_HEIGHT,(LCD_WIDTH/7)*4+0.5,_GREEN);
    Glcd_Drew_FBox(0,(LCD_WIDTH/7)*4+0.5,LCD_HEIGHT,(LCD_WIDTH/7)*5+0.5,_MAGENTA);
    Glcd_Drew_FBox(0,(LCD_WIDTH/7)*5+0.5,LCD_HEIGHT,(LCD_WIDTH/7)*6+0.5,_RED);
    Glcd_Drew_FBox(0,(LCD_WIDTH/7)*6+0.5,LCD_HEIGHT,LCD_WIDTH,_BLUE);
    Glcd_Drew_Box(0,0,LCD_HEIGHT-1,LCD_WIDTH-1,_WHITE);
}
