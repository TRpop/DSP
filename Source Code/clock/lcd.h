#ifndef __LCD_H__
#define __LCD_H__

unsigned short *lcd_addr;

void LCD_word(short Control, short Data);
void printCharLCD(const char *fmt, ...);
void OutputLcd(unsigned char *str);
void lcd_init(void);

#endif
