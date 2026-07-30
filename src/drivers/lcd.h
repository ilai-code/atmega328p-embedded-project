#ifndef LCD_H
#define LCD_H

// this file is based off of the arduino LiquidCrystal_I2C implementation found here: https://github.com/johnrickman/LiquidCrystal_I2C

#include <util/delay.h>
#include "twi_functions.h"

// LCD Base Address
// for AVR twi it is left aligned so that the last bit is the R/W bit
#define LCD_I2C_ADDR (0x27 << 1) // 0 is write and 1 is read

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define EN 0b00000100  // Enable bit
#define RW 0b00000010  // Read/Write bit
#define RS 0b00000001  // Register select bit

typedef struct {
    uint8_t cols;
    uint8_t rows;
    uint8_t displayfunction;
    uint8_t backlight;
    uint8_t displaycontrol;
    uint8_t displaymode;
} LCD;

void LCD_begin(uint8_t cols, uint8_t rows);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_cursor();
void lcd_noCursor();
void lcd_display();
void lcd_noDisplay();
void lcd_backlight();
void lcd_noBacklight();
void lcd_blink();
void lcd_noBlink();
void lcd_home();
void lcd_clear();
void lcd_print(const char* str);



#endif