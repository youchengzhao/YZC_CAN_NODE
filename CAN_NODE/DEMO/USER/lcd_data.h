#ifndef		__LCD_DATA_H__
#define 	__LCD_DATA_H__

const unsigned char DATA_BUF_32[][16] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
/*以上为"null"的点阵*/
{0x10,0x22,0x64,0x0C,0x80,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00},
{0x04,0x04,0xFE,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},
/*温度的点阵*/
};
const unsigned char DATA_BUF_16[][8]= {
{0xF0,0xF8,0x0C,0xC4,0x0C,0xF8,0xF0,0x00},
{0x03,0x07,0x0C,0x08,0x0C,0x07,0x03,0x00},
};

#endif