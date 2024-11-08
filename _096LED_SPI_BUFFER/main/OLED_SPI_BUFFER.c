/*
    1. Fundamental Command
    0   81 A[7:0]       设置对比度，范围1-256，RESET = 7Fh 
    0   A4/A5           恢复到RAM内容显示/显示屏全部打开
    0   A6/A7           正常显示/反显
    0   AE/AF           显示器关/开
    2. Scrolling Command
    0   26/27           左/右水平滚动
    0
    0
    3. Addressing Setting Command
    0   00-0F           设置页面寻址模式的列地址的低四位
                        此命令仅用于页面寻址模式
    0   10-1F           设置页面寻址模式的列地址的高四位
                        此命令仅用于页面寻址模式
    0   20 A[1:0]       设置内存寻址模式
                        A[1:0]=00b，水平寻址模式
                        A[1:0]=01b，垂直寻址模式
                        A[1:0]=10b，页面寻址模式（重置）
                        A[1:0]=11b，无效
    0   21 A[6:0] B[6:0]设置列的起始和结束地址
                        A[6:0]：列起始地址，范围:0-127d，RESET=0d
                        B[6:0]：列结束地址，范围:0-127d，RESET=127d
                        此命令仅适用于水平或垂直寻址模式
    0   22 A[2:0] B[2:0]设置页的起始和结束地址
                        A[2:0]：页起始地址，范围：0-7d,RESET=0d
                        B[2:0]：页结束地址，范围：0-7d,RESET=7d
                        此命令仅适用于水平或垂直寻址模式
    0   B0-B7           为页面寻址模式设置页面起始地址,PAGE0~PAGE7
    4. Hardware Configuration (Panel resolution & layout related) Command
    0   40-7F
    0   A0/A1
    0   A8 A[5:0]
    0   C0/C8
    0   D3 A[5:0]
    0   DA A[5:4]
    5. Timing & Driving Scheme Setting Command
    0   D5 A[7:0]       设置显示时钟分频比/振荡器频率
                        A[3:0]：定义显示时钟（DCLK）的分频比（D）：分频比=A[3:0]+1，RESET为0000b（分频比=1）
                        A[7:4]：设置振荡器频率FOSC。振荡器频率随着A[7:4]的值而增加，反之亦然。重置为1000b。范围：0000b~1111b
    0   D9 A[7:0]       设置预充电期
    0   DB A[6:4]
    0   E3              无操作命令
*/

#include "OLED_SPI_BUFFER.h"

/*宽8像素，高16像素*/

const uint8_t oled_f16x8[][16] =
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//   0
	{0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00},// ! 1
	{0x00,0x16,0x0E,0x00,0x16,0x0E,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// " 2
	{0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00},
	{0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00},// # 3
	{0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00},
	{0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00},// $ 4
	{0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00},
	{0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00},// % 5
	{0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00},
	{0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10},// & 6
	{0x00,0x00,0x00,0x16,0x0E,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// ' 7
	{0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00},
	{0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},// ( 8
	{0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00},
	{0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},// ) 9
	{0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00},
	{0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},// * 10
	{0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00},
	{0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00},// + 11
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0xB0,0x70,0x00,0x00,0x00,0x00,0x00},// , 12
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},// - 13
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},// . 14
	{0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04},
	{0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00},// / 15
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},// 0 16
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},// 1 17
	{0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},// 2 18
	{0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00},
	{0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},// 3 19
	{0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00},
	{0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},// 4 20
	{0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00},
	{0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},// 5 21
	{0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00},
	{0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},// 6 22
	{0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},// 7 23
	{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},// 8 24
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},// 9 25
	{0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},// : 26
	{0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00},
	{0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00},// ; 27
	{0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00},
	{0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00},// < 28
	{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00},
	{0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00},// = 29
	{0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00},
	{0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},// > 30
	{0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00},
	{0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00},// ? 31
	{0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00},
	{0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00},// @ 32
	{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00},
	{0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},// A 33
	{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},// B 34
	{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00},
	{0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},// C 35
	{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},// D 36
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},// E 37
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},// F 38
	{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00},
	{0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},// G 39
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},// H 40
	{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},// I 41
	{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00},
	{0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},// J 42
	{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00},
	{0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},// K 43
	{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},// L 44
	{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00},
	{0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},// M 45
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},// N 46
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},// O 47
	{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00},
	{0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},// P 48
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00},// Q 49
	{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},// R 50
	{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00},
	{0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},// S 51
	{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},// T 52
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},// U 53
	{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08},
	{0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},// V 54
	{0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00},
	{0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00},// W 55
	{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08},
	{0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},// X 56
	{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},// Y 57
	{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},// Z 58
	{0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00},
	{0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00},// [ 59
	{0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},// \ 60
	{0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00},
	{0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00},// ] 61
	{0x00,0x20,0x10,0x08,0x04,0x08,0x10,0x20},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// ^ 62
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},// _ 63
	{0x00,0x02,0x04,0x08,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// ` 64
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20},// a 65
	{0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00},
	{0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},// b 66
	{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},// c 67
	{0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20},// d 68
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00},// e 69
	{0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},// f 70
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},// g 71
	{0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00},
	{0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},// h 72
	{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},// i 73
	{0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00},
	{0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},// j 74
	{0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00},
	{0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00},// k 75
	{0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},// l 76
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},// m 77
	{0x00,0x80,0x80,0x00,0x80,0x80,0x00,0x00},
	{0x00,0x20,0x3F,0x21,0x00,0x20,0x3F,0x20},// n 78
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00},
	{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},// o 79
	{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00},
	{0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00},// p 80
	{0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80},// q 81
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00},
	{0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},// r 82
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},// s 83
	{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00},
	{0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00},// t 84
	{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00},
	{0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},// u 85
	{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80},
	{0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00},// v 86
	{0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80},
	{0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00},// w 87
	{0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00},
	{0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00},// x 88
	{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80},
	{0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00},// y 89
	{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},// z 90
	{0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02},
	{0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40},// { 91
	{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},// | 92
	{0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00},
	{0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00},// } 93
	{0x00,0x80,0x40,0x40,0x80,0x00,0x00,0x80},
	{0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00},// ~ 94
};

/*宽6像素，高8像素*/

const uint8_t oled_f8x6[][6] = 
{
	{0x00,0x00,0x00,0x00,0x00,0x00},//   0
	{0x00,0x00,0x00,0x2F,0x00,0x00},// ! 1
	{0x00,0x00,0x07,0x00,0x07,0x00},// " 2
	{0x00,0x14,0x7F,0x14,0x7F,0x14},// # 3
	{0x00,0x24,0x2A,0x7F,0x2A,0x12},// $ 4
	{0x00,0x23,0x13,0x08,0x64,0x62},// % 5
	{0x00,0x36,0x49,0x55,0x22,0x50},// & 6
	{0x00,0x00,0x00,0x07,0x00,0x00},// ' 7
	{0x00,0x00,0x1C,0x22,0x41,0x00},// ( 8
	{0x00,0x00,0x41,0x22,0x1C,0x00},// ) 9
	{0x00,0x14,0x08,0x3E,0x08,0x14},// * 10
	{0x00,0x08,0x08,0x3E,0x08,0x08},// + 11
	{0x00,0x00,0x00,0xA0,0x60,0x00},// , 12
	{0x00,0x08,0x08,0x08,0x08,0x08},// - 13
	{0x00,0x00,0x60,0x60,0x00,0x00},// . 14
	{0x00,0x20,0x10,0x08,0x04,0x02},// / 15
	{0x00,0x3E,0x51,0x49,0x45,0x3E},// 0 16
	{0x00,0x00,0x42,0x7F,0x40,0x00},// 1 17
	{0x00,0x42,0x61,0x51,0x49,0x46},// 2 18
	{0x00,0x21,0x41,0x45,0x4B,0x31},// 3 19
	{0x00,0x18,0x14,0x12,0x7F,0x10},// 4 20
	{0x00,0x27,0x45,0x45,0x45,0x39},// 5 21
	{0x00,0x3C,0x4A,0x49,0x49,0x30},// 6 22
	{0x00,0x01,0x71,0x09,0x05,0x03},// 7 23
	{0x00,0x36,0x49,0x49,0x49,0x36},// 8 24
	{0x00,0x06,0x49,0x49,0x29,0x1E},// 9 25
	{0x00,0x00,0x36,0x36,0x00,0x00},// : 26
	{0x00,0x00,0x56,0x36,0x00,0x00},// ; 27
	{0x00,0x08,0x14,0x22,0x41,0x00},// < 28
	{0x00,0x14,0x14,0x14,0x14,0x14},// = 29
	{0x00,0x00,0x41,0x22,0x14,0x08},// > 30
	{0x00,0x02,0x01,0x51,0x09,0x06},// ? 31
	{0x00,0x3E,0x49,0x55,0x59,0x2E},// @ 32
	{0x00,0x7C,0x12,0x11,0x12,0x7C},// A 33
	{0x00,0x7F,0x49,0x49,0x49,0x36},// B 34
	{0x00,0x3E,0x41,0x41,0x41,0x22},// C 35
	{0x00,0x7F,0x41,0x41,0x22,0x1C},// D 36
	{0x00,0x7F,0x49,0x49,0x49,0x41},// E 37
	{0x00,0x7F,0x09,0x09,0x09,0x01},// F 38
	{0x00,0x3E,0x41,0x49,0x49,0x7A},// G 39
	{0x00,0x7F,0x08,0x08,0x08,0x7F},// H 40
	{0x00,0x00,0x41,0x7F,0x41,0x00},// I 41
	{0x00,0x20,0x40,0x41,0x3F,0x01},// J 42
	{0x00,0x7F,0x08,0x14,0x22,0x41},// K 43
	{0x00,0x7F,0x40,0x40,0x40,0x40},// L 44
	{0x00,0x7F,0x02,0x0C,0x02,0x7F},// M 45
	{0x00,0x7F,0x04,0x08,0x10,0x7F},// N 46
	{0x00,0x3E,0x41,0x41,0x41,0x3E},// O 47
	{0x00,0x7F,0x09,0x09,0x09,0x06},// P 48
	{0x00,0x3E,0x41,0x51,0x21,0x5E},// Q 49
	{0x00,0x7F,0x09,0x19,0x29,0x46},// R 50
	{0x00,0x46,0x49,0x49,0x49,0x31},// S 51
	{0x00,0x01,0x01,0x7F,0x01,0x01},// T 52
	{0x00,0x3F,0x40,0x40,0x40,0x3F},// U 53
	{0x00,0x1F,0x20,0x40,0x20,0x1F},// V 54
	{0x00,0x3F,0x40,0x38,0x40,0x3F},// W 55
	{0x00,0x63,0x14,0x08,0x14,0x63},// X 56
	{0x00,0x07,0x08,0x70,0x08,0x07},// Y 57
	{0x00,0x61,0x51,0x49,0x45,0x43},// Z 58
	{0x00,0x00,0x7F,0x41,0x41,0x00},// [ 59
	{0x00,0x02,0x04,0x08,0x10,0x20},// \ 60
	{0x00,0x00,0x41,0x41,0x7F,0x00},// ] 61
	{0x00,0x04,0x02,0x01,0x02,0x04},// ^ 62
	{0x00,0x40,0x40,0x40,0x40,0x40},// _ 63
	{0x00,0x00,0x01,0x02,0x04,0x00},// ` 64
	{0x00,0x20,0x54,0x54,0x54,0x78},// a 65
	{0x00,0x7F,0x48,0x44,0x44,0x38},// b 66
	{0x00,0x38,0x44,0x44,0x44,0x20},// c 67
	{0x00,0x38,0x44,0x44,0x48,0x7F},// d 68
	{0x00,0x38,0x54,0x54,0x54,0x18},// e 69
	{0x00,0x08,0x7E,0x09,0x01,0x02},// f 70
	{0x00,0x18,0xA4,0xA4,0xA4,0x7C},// g 71
	{0x00,0x7F,0x08,0x04,0x04,0x78},// h 72
	{0x00,0x00,0x44,0x7D,0x40,0x00},// i 73
	{0x00,0x40,0x80,0x84,0x7D,0x00},// j 74
	{0x00,0x7F,0x10,0x28,0x44,0x00},// k 75
	{0x00,0x00,0x41,0x7F,0x40,0x00},// l 76
	{0x00,0x7C,0x04,0x18,0x04,0x78},// m 77
	{0x00,0x7C,0x08,0x04,0x04,0x78},// n 78
	{0x00,0x38,0x44,0x44,0x44,0x38},// o 79
	{0x00,0xFC,0x24,0x24,0x24,0x18},// p 80
	{0x00,0x18,0x24,0x24,0x18,0xFC},// q 81
	{0x00,0x7C,0x08,0x04,0x04,0x08},// r 82
	{0x00,0x48,0x54,0x54,0x54,0x20},// s 83
	{0x00,0x04,0x3F,0x44,0x40,0x20},// t 84
	{0x00,0x3C,0x40,0x40,0x20,0x7C},// u 85
	{0x00,0x1C,0x20,0x40,0x20,0x1C},// v 86
	{0x00,0x3C,0x40,0x30,0x40,0x3C},// w 87
	{0x00,0x44,0x28,0x10,0x28,0x44},// x 88
	{0x00,0x1C,0xA0,0xA0,0xA0,0x7C},// y 89
	{0x00,0x44,0x64,0x54,0x4C,0x44},// z 90
	{0x00,0x00,0x08,0x7F,0x41,0x00},// { 91
	{0x00,0x00,0x00,0x7F,0x00,0x00},// | 92
	{0x00,0x00,0x41,0x7F,0x08,0x00},// } 93
	{0x00,0x08,0x04,0x08,0x10,0x08},// ~ 94
};
/*********************ASCII字模数据*/

uint8_t img_face[]={
	0xFF,0X01,0XE1,0X11,0X49,0X25,0X45,0X05,0X45,0X25,0X49,0X11,0XE1,0X01,0XFF,
	0X7F,0X40,0X43,0X44,0X48,0X51,0X52,0X52,0X52,0X51,0X48,0X44,0X43,0X40,0X7F
	};

uint8_t oled_disply_buffer[8][4][32]={0};

spi_device_handle_t spi;

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(PIN_NUM_D, dc);
}

void spi_init(){
    
    for (int i = 0; i < 1000; i ++)
	{
		for (int j = 0; j < 1000; j ++);
	}

    
    spi_bus_config_t bus_cfg={
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,
    };

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 10 * 1000 * 1000,     //Clock out at 10 MHz
        .mode = 0,                              //SPI mode 0
        .spics_io_num = PIN_NUM_CS,             //CS pin
        .queue_size = 7,                        //We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    };

    ESP_ERROR_CHECK(spi_bus_initialize(OLED_HOST, &bus_cfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(OLED_HOST,&dev_cfg, &spi));

    gpio_config_t dc_cfg={
        .pin_bit_mask=(1<<GPIO_NUM_2),
        .pull_up_en=GPIO_PULLDOWN_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .intr_type=GPIO_INTR_DISABLE,
    };
    gpio_config(&dc_cfg);
    dc_cfg.pin_bit_mask=(1<<GPIO_NUM_4);
    gpio_config(&dc_cfg);  
}


void oled_reset(){
    RES_ON;
    for (int i = 0; i < 1000; i ++)
	{
		for (int j = 0; j < 1000; j ++);
	}
    RES_OFF;
}

void oled_init(){
    spi_init();

    oled_reset();

    uint8_t data[23];  //定义要发送过去的数据
	data[0] = 0xAE;     //设置显示开启/关闭，0xAE关闭，0xAF开启
	data[1] = 0xD5;     //设置显示时钟分频比/振荡器频率
	data[2] = 0x80;     //0x00~0xFF
    data[3] = 0xA8;     //设置多路复用率
    data[4] = 0x3F;     //0x0E~0x3F
    data[5] = 0xD3;     //设置显示偏移
    data[6] = 0x00;     //0x00~0x7F
    data[7] = 0x40;     //设置显示开始行，0x40~0x7F
    data[8] = 0xA1;     //设置左右方向，0xA1正常，0xA0左右反置
	data[9] = 0xC8;     //设置上下方向，0xC8正常，0xC0上下反置
	data[10] = 0xDA;    //设置COM引脚硬件配置
    data[11] = 0x12;
    data[12] = 0x81;    //设置对比度
    data[13] = 0xCF;    //0x00~0xFF
    data[14] = 0xD9;    //设置预充电周期
    data[15] = 0xF1;
    data[16] = 0xDB;    //设置VCOMH取消选择级别
    data[17] = 0x30;
    data[18] = 0xA4;    //设置整个显示打开/关闭
    data[19] = 0xA6;    //设置正常/反色显示，0xA6正常，0xA7反色
    data[20] = 0x8D;    //设置充电泵
    data[21] = 0x14;    //开启显示
    data[22] = 0xAF;


    spi_transaction_t trans_cfg; //定义数据结构体
	memset(&trans_cfg, 0, sizeof(trans_cfg));  //为数据结构体分配内存
	trans_cfg.cmd = 0x9F; //因为是固定内存地址，使用的是nterface_config的配置，也就是8bit cmd，0bit address
	trans_cfg.length = 23 * 8;  //要发送或者接收的数据的长度，不算前面的cmd/address/dummy的长度
	trans_cfg.tx_buffer =data;  //发送没有指定内部空间，使用的是外部区域，因此要自己指定
	trans_cfg.rx_buffer = NULL; //接收定义了SPI_TRANS_USE_RXDATA，使用的是内部空间。
	trans_cfg.flags = SPI_TRANS_USE_RXDATA;


    spi_device_polling_transmit(spi, &trans_cfg);
}



void oled_write_byte(uint8_t chr){
    spi_transaction_t trans_cfg; //定义数据结构体
    memset(&trans_cfg, 0, sizeof(trans_cfg));  //为数据结构体分配内存
    trans_cfg.cmd = 0x9F; //因为是固定内存地址，使用的是nterface_config的配置，也就是8bit cmd，0bit address
    trans_cfg.length = 1 * 8;  //要发送或者接收的数据的长度，不算前面的cmd/address/dummy的长度
    trans_cfg.tx_buffer =&chr;  //发送没有指定内部空间，使用的是外部区域，因此要自己指定
    trans_cfg.rx_buffer = NULL; //接收定义了SPI_TRANS_USE_RXDATA，使用的是内部空间。
    trans_cfg.flags = SPI_TRANS_USE_RXDATA;

    spi_device_polling_transmit(spi, &trans_cfg);
}

void oled_write_bytes(uint8_t chr[],uint8_t n){
    spi_transaction_t trans_cfg; //定义数据结构体
    memset(&trans_cfg, 0, sizeof(trans_cfg));  //为数据结构体分配内存
    trans_cfg.cmd = 0x9F; //因为是固定内存地址，使用的是nterface_config的配置，也就是8bit cmd，0bit address
    trans_cfg.length = n * 8;  //要发送或者接收的数据的长度，不算前面的cmd/address/dummy的长度
    trans_cfg.tx_buffer =chr;  //发送没有指定内部空间，使用的是外部区域，因此要自己指定
    trans_cfg.rx_buffer = NULL; //接收定义了SPI_TRANS_USE_RXDATA，使用的是内部空间。
    trans_cfg.flags = SPI_TRANS_USE_RXDATA;

    spi_device_polling_transmit(spi, &trans_cfg);
}


void oled_set_cursor(uint8_t Page,uint8_t Coul){
    DC_Command;
    uint8_t xy[]={0x00|(Coul&0x0F),0x10|((Coul&0xF0)>>4),0xB0|Page};
    oled_write_bytes(xy,3);
}

uint8_t clr[4][32]={0};
void oled_clr(){
	for (uint8_t i = 0; i < 8; i++)
    {
		oled_set_cursor(i,0);
        DC_Data;
		for (uint8_t i = 0; i < 4; i++)
		{
			oled_write_bytes(clr[i],32);
		}
	}
}



/*
void oled_clr(){
	for (uint8_t i = 0; i < 8; i++)
    {
        oled_set_cursor(i,0);
        DC_Data;
        for (uint8_t j = 0; j < 128; j++)
        {
            oled_write_byte(0x00);
	    }
    }
}
*/
void oled_write_buffer(uint8_t Page,uint8_t Coul,uint8_t data,uint8_t dire,uint8_t bits){
	if (Page<8 && Coul<128)		
	{
		switch (dire)
		{
		case 1:
			oled_disply_buffer[Page][Coul/32][Coul%32]|= data<<bits;
			break;
		case 0:
			oled_disply_buffer[Page][Coul/32][Coul%32]|= data>>bits;
			break;
		default:
			break;
		}
	}
	
	
}

void oled_update(){
	for (uint8_t i = 0; i < 8; i++)
    {
		oled_set_cursor(i,0);
        DC_Data;
		for (uint8_t j = 0; j < 4; j++)
		{
			oled_write_bytes(oled_disply_buffer[i][j],32);
		}
	}
}



void oled_witre_char(uint8_t Page,uint8_t Coul,uint8_t chr,uint8_t width){
    switch (width)
    {
    case 6://字符宽度6
        for (uint8_t i = 0; i < 6; i++)
        {
            oled_write_buffer(Page,Coul+i,oled_f8x6[chr-' '][i],0,0);
        }
        break;
	case 8://字符宽度8
        for (uint8_t i = 0; i < 8; i++)
        {
            oled_write_buffer(Page,Coul+i,oled_f16x8[(chr-' ')*2][i],0,0);
			oled_write_buffer(Page+1,Coul+i,oled_f16x8[(chr-' ')*2+1][i],0,0);
        }
        break;
	
    default:
        break;
    }
}

void oled_witre_string(uint8_t Page,uint8_t Coul,char *str,uint8_t width){
    for (uint8_t i = 0;str[i]!='\0'; i++)
    {
        oled_witre_char(Page,Coul+i*width,str[i],width);
    }
}

void oled_witre_image(uint8_t x,uint8_t y,uint8_t height,uint8_t width,uint8_t *img){
	for (uint8_t i = 0; i < width/8+1; i++){
		for (uint8_t j = 0; j < width; j++)
		{
			oled_write_buffer(y/8+i,x+j,img[j+i*15],1,y%8);
			if(y%8!=0){oled_write_buffer(y/8+1+i,x+j,img[j+i*15],0,8-(y%8));}
			
		}
	}

}
