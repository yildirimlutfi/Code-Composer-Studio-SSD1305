/* SSD1305
 * PINs and MATRIX STRUCTURE
 *
 *   0 1 2 ...............127
 *  0 .......................
 *  1 .......................
 *  2 .......................
 *  .........................
 *  .........................
 *  .........................
 *  63.......................
 *
 *  °  °   °   °   °   °  °
 *  CS-DC-RST-SDA-SCL-VDD-GND
 *
 *
 *
 *
void exampleOfDisplay(uint8_t startX, uint8_t startY, uint8_t finishX, uint8_t finishY)
{
    int i;
    if (startX>127) startX=127;
    if (finishX>127) finishX=127;

    if (startY>63) startX=63;
    if (finishY>63) finishX=63;


    for(i=startX; i <finishX; i++)
    {
        command(SSD1305_SETCOLADDR);
        command(i);//start column
        command(i);//finish column
        data(0b01111111);
        data(0b00111111);
        data(0b00011111);
        data(0b0001111);
        data(0b11110000);
        data(0b11111000);
        data(0b11111100);
        data(0b11111110);
    }
}
 *
 *
 * */

#include "Board.h"
#include "String.h"
#include <stdio.h>
#include "font.h"
#include "imagesOfDisplay.h"

/* Pin driver handle */
static PIN_Handle PinHandle;
static PIN_State PinState;
unsigned char Data_BUS;

#define SPI_CS  IOID_20
#define SPI_DC  IOID_9
#define IC_RST IOID_13
#define SPI_SDA IOID_14
#define SPI_SCK IOID_10

PIN_Config PinTable[] = {

                         SPI_CS       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                         IC_RST       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                         SPI_DC       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                         SPI_SDA      | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                         SPI_SCK      | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

#define WIDTH 128
#define HEIGHT 64

#define SSD1305_SETLOWCOLUMN 0x00
#define SSD1305_SETHIGHCOLUMN 0x10
#define SSD1305_MEMORYMODE 0x20
#define SSD1305_SETCOLADDR 0x21
#define SSD1305_SETPAGEADDR 0x22
#define SSD1305_SETSTARTLINE 0x40
#define SSD1305_SETCONTRAST 0x81
#define SSD1305_SETBRIGHTNESS 0x82
#define SSD1305_SETLUT 0x91
#define SSD1305_SEGREMAP 0xA0
#define SSD1305_DISPLAYALLON_RESUME 0xA4
#define SSD1305_DISPLAYALLON 0xA5
#define SSD1305_NORMALDISPLAY 0xA6
#define SSD1305_INVERTDISPLAY 0xA7
#define SSD1305_SETMULTIPLEX 0xA8
#define SSD1305_DISPLAYDIM 0xAC
#define SSD1305_MASTERCONFIG 0xAD
#define SSD1305_DISPLAYOFF 0xAE
#define SSD1305_DISPLAYON 0xAF
#define SSD1305_SETPAGESTART 0xB0
#define SSD1305_COMSCANINC 0xC0
#define SSD1305_COMSCANDEC 0xC8
#define SSD1305_SETDISPLAYOFFSET 0xD3
#define SSD1305_SETDISPLAYCLOCKDIV 0xD5
#define SSD1305_SETAREACOLOR 0xD8
#define SSD1305_SETPRECHARGE 0xD9
#define SSD1305_SETCOMPINS 0xDA
#define SSD1305_SETVCOMLEVEL 0xDB
#define SSD1305_ENTERREADMODIFYWRITEMODE 0xE0
#define SSD1305_NOP 0xE3

uint8_t buffer[WIDTH][HEIGHT];

void delay(uint64_t delay_value)
{
    while(delay_value!=0)
    delay_value--;
}

void delay1(uint64_t delay_value)
{
    unsigned int i,j;

    for(i=0;i<=delay_value;i++)
    {
        for(j=0;j<=delay_value;j++)
        {}
    }
}

void SPItransfer(unsigned char dat)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        if((dat)&0x80>>i)
        {
            PIN_setOutputValue(PinHandle, SPI_SDA,1);
        }
        else
        {
            PIN_setOutputValue(PinHandle, SPI_SDA,0);
        }
        delay(1);
        PIN_setOutputValue(PinHandle, SPI_SCK,1);
        delay(1);
        PIN_setOutputValue(PinHandle, SPI_SCK,0);
    }

}

void command(uint8_t cmd)
{
    PIN_setOutputValue(PinHandle, SPI_DC,0);
    SPItransfer(cmd);
}

void oledInit()
{
    PinHandle = PIN_open(&PinState, PinTable);
    PIN_setOutputValue(PinHandle, SPI_CS,0);
    PIN_setOutputValue(PinHandle, IC_RST,1);
    delay(10);
    PIN_setOutputValue(PinHandle, IC_RST,0);
    delay(10);
    PIN_setOutputValue(PinHandle, IC_RST,1);

    command(SSD1305_DISPLAYOFF);
    command(SSD1305_MEMORYMODE);
    command(0x00);
    command(SSD1305_SETCONTRAST);
    command(0xFF);// 0x81, 0x32
    command(SSD1305_SETBRIGHTNESS);
    command(0x80);// 0x82, 0x80
    command(SSD1305_SEGREMAP | 0x01);
    command(SSD1305_NORMALDISPLAY);
    command(SSD1305_SETMULTIPLEX);
    command(0x3F);// 0xA8, 0x3F (1/64)
    command(SSD1305_MASTERCONFIG);
    command(0x8E);/* external vcc supply */
    command(SSD1305_COMSCANDEC);//SSD1305_COMSCANINC
    command(SSD1305_SETDISPLAYOFFSET);
    command(0x40);// 0xD3, 0x3F
    command(SSD1305_SETDISPLAYCLOCKDIV);
    command(0xF0);// 0xD5, 0xF0
    command(SSD1305_SETAREACOLOR & 0xC8);
    command(0x05);
    command(SSD1305_SETCOMPINS);
    command(0x12);// 0xDA, 0x12
    command(SSD1305_SETLUT);
    command(0x3F);
    command(0x3F);
    command(0x3F);
    command(0x3F);
    delay(100);
    command(SSD1305_DISPLAYON);
    setContrast(0x01);
}

void data(uint8_t cmd)
{
    PIN_setOutputValue(PinHandle, SPI_DC,1);
    SPItransfer(cmd);
}

void setContrast(unsigned char level)
{
    SPItransfer(SSD1305_SETCONTRAST);
    SPItransfer(level);
}

void inverseDisplay()
{
    command(SSD1305_INVERTDISPLAY);
}

void normalDisplay()
{
    command(SSD1305_NORMALDISPLAY);
}

void scrool()
{
    command(0x26);
    command(0b100);
    command(0b000);
    command(0b000);
    command(0b111);


}

void displayBuffer()
{
///////////////////////////////////////////////THE DISPLAY ROTATED 180° /////////////////////////////////
//if you want real coordinate comment this part open other part
    int i,j;
    for(i=0;i<WIDTH;i++)
    {
        command(SSD1305_SETCOLADDR);
        command(i);//start column
        command(i);//finish column
        for(j=HEIGHT-1;j>0;j=j-8)
        {
            data((buffer[127-i][j])+(buffer[127-i][j-1]<<1) +(buffer[127-i][j-2]<<2)+(buffer[127-i][j-3]<<3)+(buffer[127-i][j-4]<<4)+(buffer[127-i][j-5]<<5)+(buffer[127-i][j-6]<<6)+(buffer[127-i][j-7]<<7));
        }
    }

///////////////////////////////////////////////THE DISPLAY's REAL COORDINATE /////////////////////////////////
//    int i,j;
//    for(i=0;i<WIDTH;i++)
//        //for(i=0;i<WIDTH;i++)
//    {
//        command(SSD1305_SETCOLADDR);
//        command(i);//start column
//        command(i);//finish column
//        for(j=0;j<HEIGHT;j=j+8)
//        {
//            data((buffer[i][j+8]<<7)+(buffer[i][j+7]<<6) +(buffer[i][j+6]<<5)+(buffer[i][j+5]<<4)+(buffer[i][j+4]<<3)+(buffer[i][j+3]<<2)+(buffer[i][j+2]<<1)+(buffer[i][j+1]));
//        }
//    }

}

void clearDisplay()
{
    int i,j;
    for(i=0;i<128;i++)
    {
        command(SSD1305_SETCOLADDR);
        command(i);//start column
        command(i);//finish column
        for(j=0;j<8;j++)
        {
            data(0x00);
        }
    }
}

void clearArea(uint8_t startPixelX, uint64_t startPixelY, uint8_t width, uint64_t height)
{
    if(startPixelX>127 || startPixelY>63 || startPixelX+width>128 || startPixelY+height>64)
      return;

    int i,j;
    for(i=startPixelX;i<startPixelX+width;i++)
    {
        for(j=startPixelY;j<startPixelY+height;j++)
        {
            buffer[i][j]=0;
        }
    }
    displayBuffer();
}

void drawLine(uint8_t startPixelX, uint8_t startPixelY, uint8_t finishPixelX, uint8_t finishPixelY)
{
    if(startPixelX>127 || startPixelY>63 || finishPixelX>127 || finishPixelY>63)
        return;

    float slope;
    float temp;
    int i,j;

    if(startPixelX==finishPixelX)
        for(j=startPixelY;j<finishPixelY;j++)
        {
            buffer[startPixelX][j]=1;
        }
    else if(startPixelY==finishPixelY)
        for(i=startPixelX;i<finishPixelX;i++)
        {
            buffer[i][startPixelY]=1;
        }
    else
    {//this part doesn't work completely
        slope= ((float)finishPixelY-(float)startPixelY) / ((float)finishPixelX-(float)startPixelX);
        buffer[startPixelX][startPixelY]=1;
        buffer[finishPixelX][finishPixelY]=1;
        for(i=startPixelX;i<finishPixelX+1;i++)
        {
            for(j=startPixelY;j<finishPixelY+1;j++)
            {
                temp=(float)i*slope;
                if(((int)temp)==((int)j))
                {
                    buffer[i][j]=1;
                }
            }
        }
    }
}

void drawFont_5x8(uint8_t startPixelX, uint8_t startPixelY, char character)
{
    if ((startPixelX > 128) || (startPixelY > 64))
      return;

    int i,j;
    for(i=startPixelX;i<startPixelX+5;i++)
    {
        for(j=startPixelY;j<startPixelY+1;j++)
        {
            buffer[i][j+0]=(Font_5x8[character][i-startPixelX]>>0)&0x01;
            buffer[i][j+1]=(Font_5x8[character][i-startPixelX]>>1)&0x01;
            buffer[i][j+2]=(Font_5x8[character][i-startPixelX]>>2)&0x01;
            buffer[i][j+3]=(Font_5x8[character][i-startPixelX]>>3)&0x01;
            buffer[i][j+4]=(Font_5x8[character][i-startPixelX]>>4)&0x01;
            buffer[i][j+5]=(Font_5x8[character][i-startPixelX]>>5)&0x01;
            buffer[i][j+6]=(Font_5x8[character][i-startPixelX]>>6)&0x01;
            buffer[i][j+7]=(Font_5x8[character][i-startPixelX]>>7)&0x01;
        }
    }
}

void drawString(uint8_t startPixelX, uint8_t startPixelY, char character[])
{
    if ((startPixelX > 128) || (startPixelY > 64))
        return;

    int i=0;
    while (character[i]!='\0')
    {
        drawFont_5x8(startPixelX+(i*7),startPixelY,character[i]);
        i++;
    }
    displayBuffer();
}

void drawNumberDecimal(uint8_t startPixelX, uint8_t startPixelY, int64_t number)
{
    char temp[10];
    sprintf(temp, "%d", number);
    drawString(startPixelX,startPixelY,temp);
}

void drawImage(uint8_t startPixelX, uint8_t startPixelY,const uint8_t *bitmap, uint8_t imagePixelWidth, uint8_t imagePixelHeight)
{
    if(startPixelX>127 || startPixelY>63 || startPixelX+imagePixelWidth>128 || startPixelY+imagePixelHeight>64)
      return;

    int i,j;
    for(i=startPixelX;i<startPixelX+imagePixelWidth;i++)
    {
        for(j=startPixelY;j<startPixelY+imagePixelHeight;j=j+8)
        {
            buffer[i][j+0]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>7)&0x01;
            buffer[i][j+1]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>6)&0x01;
            buffer[i][j+2]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>5)&0x01;
            buffer[i][j+3]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>4)&0x01;
            buffer[i][j+4]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>3)&0x01;
            buffer[i][j+5]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>2)&0x01;
            buffer[i][j+6]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>1)&0x01;
            buffer[i][j+7]=(bitmap[((i-startPixelX) * (imagePixelHeight/8) ) + (j/8)]>>0)&0x01;
        }
    }
    displayBuffer();
}
