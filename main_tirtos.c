#include <stdio.h>
#include <SSD1305.h>

int64_t i;


void logo(bool visible)
{
    if(visible)
        drawImage(0, 0, acMonogram, 24, 16);
    else
        clearArea(0, 0, 24, 16);
}

void warning(bool visible)
{
    if(visible)
        drawImage(80, 0, warningIcon, 8, 8);
    else
        clearArea(80, 0, 8, 8);
}

void wireless(uint8_t wirelessStatus)
{
    if(wirelessStatus==1)//signal smooth
        drawImage(90, 0, wirelessIconFull, 13, 8);
    else if(wirelessStatus==2)//low signal
        drawImage(90, 0, wirelessIconLow, 13, 8);
    else//no signal
        drawImage(90, 0, wirelessIconNoConnection, 13, 8);
}

void battery(uint8_t level)
{
    if(level==4)
        drawImage(110, 0, battery4, 16, 8);
    else if(level==3)
        drawImage(110, 0, battery3, 16, 8);
    else if(level==2)
        drawImage(110, 0, battery2, 16, 8);
    else if(level==1)
        drawImage(110, 0, battery1, 16, 8);
    else
        drawImage(110, 0, battery0, 16, 8);
}



void body(uint8_t loadCell1,uint8_t loadCell2)
{
    drawString(0,20,"KANCA");
    drawString(47,20,"KEDI");
    drawString(90,20,"KOPRU");

    drawString(0,31,"AKTIF"); drawString(47,31,"AKTIF"); drawString(90,31,"PASIF");
    drawString(0,31,"PASIF"); drawString(47,31,"AKTIF"); drawString(90,31,"HATA ");

    drawString(0,45,"YUK 1(kg): "); drawNumberDecimal(73, 45, loadCell1);
    drawString(0,55,"YUK 2(kg): "); drawNumberDecimal(73, 55, loadCell2%5);

    drawLine(39, 17, 39, 41);
    drawLine(84, 17, 84, 41);
}

void main()
{
    Board_init();
    oledInit();
    clearDisplay();
    logo(true);
//    drawLine(0, 0, 127, 63);
    
    while(1)
    {
        for(i=0;i<1000;i++)
        {
            if(i%3==0)
            {
                warning(false);
                wireless(1);
            }
            else if (i%3==1)
            {
                warning(false);
                wireless(2);
            }
            else
            {
                warning(true);
                wireless(0);
            }
            battery(i%5);
            body(i,i);
            CPUdelay(1000000);

        }
    }
}
