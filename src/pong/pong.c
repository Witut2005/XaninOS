
#include <pong/pong.h>
#include <lib/xstack.h>
#include <xaninGraphics/xaninGraphics.c>





void keyControlPong(void)
{
    asm("nop");
}



void createRectanglePong(uint8_t ySize,uint8_t xwhere,uint8_t ywhere,uint8_t id)
{   
    uint32_t* lineCreator = (uint32_t*)VGA_TEXT_MEMORY + xwhere + (ywhere * 40);


    uint32_t* buf = lineCreator;
    
    uint32_t** pixel;

    if(id == 1)
        pixel = pixels1;
    else if(id == 2)
        pixel = pixels2;



    for(int j = 1; j <= ySize; j++)
    {
        pixel[j - 1] = lineCreator;
        *lineCreator = (uint32_t) (( 0x0 | (((white << 4) | white) << 8)) << 16) | ( 0x0 | (((white << 4) | white) << 8));
        lineCreator = lineCreator + 40;
    }

    lineCreator = buf;



}


void resetBall(void)
{
    if(keyboard_input == 'r')
    {
        vector = 0x0;
        *ball = 0x0;
        ball = (uint16_t*)VGA_TEXT_MEMORY + 40 + (80 * 12);
    }
}

void chgRelativeLocationY(uint8_t ynew,uint8_t id)
{

    uint32_t** pixel;

    if(id == 1)
    {
        player1Buf = keyboard_input;
        pixel = pixels1;
    }
    
    else if(id == 2)
    {
        player2Buf = keyboard_input;
        pixel = pixels2;
    }


    if((keyboard_input == player1[0] && id == 1)|| (keyboard_input == player2[0] && id == 2))
    {
        
        uint32_t* tmp = pixel[0];

        tmp = tmp - 40;


        if (*tmp == (uint32_t) (((0x0 | (((lgreen << 4) | white) << 8) ) | ((0x0 | (((lgreen << 4) | white) << 8)) << 16))))
            goto loop;

        for(int i = 0; i < 5; i++)
        {
            *pixel[i] = 0x0;
            pixel[i] = pixel[i] - (40 * ynew);
            *pixel[i] = (uint32_t) (( 0x0 | (((white << 4) | white) << 8)) << 16) | ( 0x0 | (((white << 4) | white) << 8));
        }


    
    }

   if((keyboard_input == player1[1] && id == 1)|| (keyboard_input == player2[1] && id == 2))
    {

        uint32_t* tmp = pixel[4];

        tmp = tmp + 40;


        if (*tmp == (uint32_t) (((0x0 | (((lgreen << 4) | white) << 8) ) | ((0x0 | (((lgreen << 4) | white) << 8)) << 16))))
            goto loop;



        for(int i = 4; i >= 0; i--)
        {
            *pixel[i] = 0x0;
            pixel[i] = pixel[i] + (40 * ynew);
            *pixel[i] = (uint32_t) (( 0x0 | (((white << 4) | white) << 8)) << 16) | ( 0x0 | (((white << 4) | white) << 8));
        }
    }




    loop:
    for(int i = 0; i < 300; i++)
        for(int j = 0; j < cpu_mhz;j++)
            asm("nop");


}

bool detectColission(void)
{


    uint16_t* tmp = ball;

    if(dest.changeDirection)
        tmp++;

    else 
        tmp--;


    if(*tmp == (uint16_t)(( 0x0 | (((white << 4) | white) << 8))))
    {
        dest.changeDirection = !dest.changeDirection;
        getTime();
        vector = time.seconds % 3;
    }

    if(*tmp == (uint16_t)(( 0x0 | (((red << 4) | white) << 8))))
    {
        app_exited = true;
    }


    if(dest.changeDirection)
    {

        tmp = ball;
        tmp += 80;

        if(*tmp == (uint16_t)(0x0 | (((lgreen << 4) | white) << 8)))
            {velocity = velocity - 160;}//asm("int 0x0");}
    
        tmp = ball;
        tmp -= 80;

        if(*tmp == (uint16_t)(0x0 | (((lgreen << 4) | white) << 8)))
           {velocity = velocity + 160;}//asm("int 0x0");}

    }


    else
    {
        tmp = ball;
        tmp += 80;

        if(*tmp == (uint16_t)(0x0 | (((lgreen << 4) | white) << 8)))
            {velocity = velocity + 160;}//asm("int 0x0");}
    
        tmp = ball;
        tmp -= 80;

        if(*tmp == (uint16_t)(0x0 | (((lgreen << 4) | white) << 8)))
           {velocity = velocity - 160;}//asm("int 0x0");}
    }

    *ball = (uint16_t)(( 0xa | (((white << 4) | white) << 8)));

}

void moveBall(void)
{
        
    uint16_t buf;

    detectColission();

    getTime();

    uint8_t move = time.seconds % 2;

    if(!vector)
    {

        velocity = 0x1;

        buf = *ball;

        *ball = 0x0;
        
        if(dest.changeDirection)
            ball = ball + velocity;

        else 
            ball = ball - velocity;

        *ball = buf;
   

    }



    else
    {
        buf = *ball;

        *ball = 0x0;

        if(dest.changeDirection)
            ball = ball + velocity + (int16_t)80;

        else 
        {
            ball = ball - velocity - (int16_t)80;
        }

        *ball = buf;
    }


}


void initPong()
{
        
//        int32_63 = 0x2;
      //signal.appHandler = &keyControlPong;

        KEYBOARD_SIG_ON(keyControlPong);
        
        clearScr();


        ball  = (uint16_t*)VGA_TEXT_MEMORY + 40 + (80 * 12);    
        

        velocity = 0x1;
        vector = 0x0;
        dest.changeDirection = 0x1;

    
        drawLineY(1,23,0,red);

        
        drawLineY(1,23,79,red);


        createRectanglePong(5,4,10,1);

        createRectanglePong(5,75,10,2);


        for(int i = 0; i < 2; i ++)
        {
            drawLineX(0,79,i,lgreen);
            drawLineX(0,79,23+i,lgreen);
        }
}


void updatePong(void)
{
    if(keyboard_input == 'q')
        {app_exited = true;}
    resetBall();
    moveBall();
    chgRelativeLocationY(1,2);
    chgRelativeLocationY(1,1);
}