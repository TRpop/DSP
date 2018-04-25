#include <lib.h>
#include <serial.h>
#include <time.h>

#define DOTLED_ROW 0x1480c000
#define DOTLED_COL 0x1480a000
#define DOTLED_COL1 0x1480b000

#define SCENCE_NUM 13
#define COL_NUM 10


void c_main(void)
{
    unsigned short row_data;
    int i,j;
    char ch;
    unsigned short dispdata[20];
    unsigned short *addr_row, *addr_col, *addr_col1;
    unsigned short data[SCENCE_NUM][COL_NUM];
data[0][0] =0x1C, data[0][1] =0x14, data[0][2] =0x1C, data[0][3] =0x08, data[0][4] =0x3E,
data[0][5] =0x08, data[0][6] =0x08, data[0][7] =0x1C, data[0][8] =0x14, data[0][9] =0x14,
data[1][0] =0x1C, data[1][1] =0x14, data[1][2] =0x1C, data[1][3] =0x08, data[1][4] =0x3E,
data[1][5] =0x08, data[1][6] =0x08, data[1][7] =0x1C, data[1][8] =0x14, data[1][9] =0x14,
data[2][0] =0x1C, data[2][1] =0x14, data[2][2] =0x1C, data[2][3] =0x08, data[2][4] =0x3E,
data[2][5] =0x08, data[2][6] =0x08, data[2][7] =0x1C, data[2][8] =0x14, data[2][9] =0x14,
data[3][0] =0x1C, data[3][1] =0x14, data[3][2] =0x1C, data[3][3] =0x08, data[3][4] =0x3E,
data[3][5] =0x08, data[3][6] =0x08, data[3][7] =0x1C, data[3][8] =0x14, data[3][9] =0x14,
data[4][0] =0x1C, data[4][1] =0x14, data[4][2] =0x1C, data[4][3] =0x08, data[4][4] =0x3E,
data[4][5] =0x08, data[4][6] =0x08, data[4][7] =0x1C, data[4][8] =0x14, data[4][9] =0x14,
data[5][0] =0x1C, data[5][1] =0x14, data[5][2] =0x1C, data[5][3] =0x08, data[5][4] =0x3E,
data[5][5] =0x08, data[5][6] =0x08, data[5][7] =0x1C, data[5][8] =0x14, data[5][9] =0x14,
data[6][0] =0x1C, data[6][1] =0x14, data[6][2] =0x1C, data[6][3] =0x08, data[6][4] =0x3E,
data[6][5] =0x08, data[6][6] =0x08, data[6][7] =0x1C, data[6][8] =0x14, data[6][9] =0x14,
data[7][0] =0x1C, data[7][1] =0x14, data[7][2] =0x1C, data[7][3] =0x08, data[7][4] =0x3E,
data[7][5] =0x08, data[7][6] =0x08, data[7][7] =0x1C, data[7][8] =0x14, data[7][9] =0x14,
data[8][0] =0x1C, data[8][1] =0x14, data[8][2] =0x1C, data[8][3] =0x08, data[8][4] =0x3E,
data[8][5] =0x08, data[8][6] =0x08, data[8][7] =0x1C, data[8][8] =0x14, data[8][9] =0x14,
data[9][0] =0x1C, data[9][1] =0x14, data[9][2] =0x1C, data[9][3] =0x08, data[9][4] =0x3E,
data[9][5] =0x08, data[9][6] =0x08, data[9][7] =0x1C, data[9][8] =0x14, data[9][9] =0x14,
data[10][0] =0x1C, data[10][1] =0x14, data[10][2] =0x1C, data[10][3] =0x08, data[10][4] =0x3E,
data[10][5] =0x08, data[10][6] =0x08, data[10][7] =0x1C, data[10][8] =0x14, data[10][9] =0x14,
data[11][0] =0x1C, data[11][1] =0x14, data[11][2] =0x1C, data[11][3] =0x08, data[11][4] =0x3E,
data[11][5] =0x08, data[11][6] =0x08, data[11][7] =0x1C, data[11][8] =0x14, data[11][9] =0x14,
data[12][0] =0x1C, data[12][1] =0x14, data[12][2] =0x1C, data[12][3] =0x08, data[12][4] =0x3E,
data[12][5] =0x08, data[12][6] =0x08, data[12][7] =0x1C, data[12][8] =0x14, data[12][9] =0x14

	unsigned short scene[COL_NUM];
        
    addr_row=(unsigned short *)DOTLED_ROW;
    addr_col=(unsigned short *)DOTLED_COL;
    addr_col1=(unsigned short *)DOTLED_COL1;
    
	while(1){	
		for(i=0;i<SCENE_NUM;i++)
			scene = data[i];

		for(i=0;i<100;i++)
		{
			row_data=0x01;

			for(j=0;j<COL_NUM;j++)
			{
				*addr_col=row_data;
				*addr_col1=row_data;
				*addr_row=scene[j];
				row_data*=2;
				Wait(50);    // 50us delay
			}
		}

		if (GetChar(&ch) ==1) {
			break;
		}
	}

}


