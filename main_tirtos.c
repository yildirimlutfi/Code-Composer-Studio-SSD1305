#include <stdio.h>
#include <SSD1305.h>

int64_t i;

void main()
{
    Board_init();
    oledInit();

    clearDisplay();
    drawLine(0,40,128,40);
    drawLine(95,0,95,64);

    drawString(0,0,"HELLO");
    drawString(0,10,"EVERYONE");
    drawString(0,20,"FROM LUTFI");

    while(1)
    {
        for(i=0;i<1000;i++)
        {
            drawNumber(97, 43, i);
            CPUdelay(1000000);
        }
    }
}

