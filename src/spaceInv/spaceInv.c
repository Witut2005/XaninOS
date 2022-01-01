
#include "./spaceInv.h"



void spaceKeybaord()
{
    asm("nop");
}

uint32_t** createEnemy(void)
{


    uint8_t enemColor;

    getTime();
    enemColor = 1 +(time.seconds % 15);




    nextEnemy += 5 + enemColor;


    *nextEnemy = (uint32_t) (( 0x0 | (((enemColor << 4) | white) << 8)) << 16) | ( 0x0 | (((enemColor << 4) | white) << 8));;



}


void initSpace(void)
{

    clearScr();

    IRQ1_ON(spaceKeybaord);

    drawLineX(0,79,0,lgreen);
    drawLineX(0,79,24,lgreen);

    drawLineY(0,24,0,lgreen);
    drawLineY(0,24,79,lgreen);


    for(int i = 0; i < 20; i++)
    {
        createEnemy();

        for(int h = 0; h < 2500; h++)
            for(int j = 0; j < cpu_mhz; j++)
                asm("nop");
    }
    

}