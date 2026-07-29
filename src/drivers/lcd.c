#include "lcd.h"

static LCD lcd;

void LCD_init(uint8_t cols, uint8_t rows){
    lcd.cols = cols;
    lcd.rows = rows;
    lcd.displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    lcd.backlight = LCD_NOBACKLIGHT;
}

void lcd_send_4b(uint8_t nibble, uint8_t flags){
    uint8_t payload = (nibble & 0xF0) | flags | lcd.backlight;

    uint8_t packet[3];
    packet[0] = payload | EN;
    packet[1] = payload | EN;
    packet[2] = payload & ~EN;

    twi_send(LCD_I2C_ADDR, packet, 3);
    _delay_us(50);
}

void lcd_send(uint8_t value, uint8_t mode){
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;

    lcd_send_4b(highnib, mode);
    lcd_send_4b(lownib, mode);
    _delay_us(50);
}

void lcd_command(uint8_t cmd){
    lcd_send(cmd, 0);
}

void expanderWrite(uint8_t data){
    data |= lcd.backlight;
    twi_send(LCD_I2C_ADDR, &data, 1);
}

void display(){
    lcd.displaycontrol |= LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void noDisplay(){
    lcd.displaycontrol &= ~LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void clear(){
    lcd_command(LCD_CLEARDISPLAY);
    _delay_us(2000);
}

void home(){
    lcd_command(LCD_RETURNHOME);
    _delay_us(2000);
}

void backlight(){
    lcd.backlight = LCD_BACKLIGHT;
    expanderWrite(0);
}

void noBacklight(){
    lcd.backlight = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void blink(){
    lcd.displaycontrol |= LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void noBlink(){
    lcd.displaycontrol &= ~LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void cursor(){
    lcd.displaycontrol |= LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void noCursor(){
    lcd.displaycontrol &= ~LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

void setCursor(uint8_t col, uint8_t row){
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= 4){
        row -= 1;
    }
    lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_begin(uint8_t cols, uint8_t rows){
    LCD_init(cols, rows);
    if (rows > 1){
        lcd.displayfunction |= LCD_2LINE;
    }
    _delay_ms(50);

    expanderWrite(lcd.backlight);
    _delay_ms(1000);

    lcd_send_4b(0x03 << 4, LCD_4BITMODE);
    _delay_us(4500);

    lcd_send_4b(0x03 << 4, LCD_4BITMODE);
    _delay_us(4500);

    lcd_send_4b(0x03 << 4, LCD_4BITMODE);
    _delay_us(4500);

    lcd_send_4b(0x02 << 4, LCD_4BITMODE);

    // set # lines, font size, etc.
    lcd_command(LCD_FUNCTIONSET | lcd.displayfunction);

    // turn the display on with no cursor or blinking default
    lcd.displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear the screen
    clear();

    lcd.displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    lcd_command(LCD_ENTRYMODESET | lcd.displaymode);

    home();

}

void lcd_print(const char* str){
    uint8_t count = 0;
    while (*str){
        if (count == lcd.cols){
            setCursor(0, 1);
        }
        else if (count == (lcd.cols * lcd.rows)){
            clear();
            setCursor(0, 0);
            count = 0;
        }
        lcd_send(*str++, RS);
        count++;
        _delay_us(100000);
    }
}