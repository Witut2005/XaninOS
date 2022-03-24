

#include <xaninApps/pong/pong.h>
#include <lib/stdlibx.h>
#include <lib/stdiox.h>
#include <xaninGraphics/xaninGraphics.c>

void pong_init(void)
{


    screen_clear();

    drawLineX(0,79,0,lgreen);
    drawLineX(0,79,27,lgreen);

    drawLineY(0,27,0,green);
    drawLineY(0,27,79,green);


    keyboard_handle = pong_input;

    for(int i = 0; i < 6; i++)
    {
        pong1[i] = &Screen.cursor[10 + i][5];
        *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));

        pong2[i] = &Screen.cursor[10 + i][74];
        *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));

    }
    
    pong1_y = 10;
    pong2_y = 10;
    ball = &Screen.cursor[10][39];

    *ball = (' ' + (((white << 4) | white) << 8));

}

void pong_input(void)
{   

    drawLineX(0,79,0,lgreen);
    drawLineX(0,79,27,lgreen);

    drawLineY(0,27,0,green);
    drawLineY(0,27,79,green);

    if(KeyInfo.scan_code == W_KEY)
    {
        if(pong1_y == 1)
            return;

        for(int i = 0; i < 6; i++)
        {
            //BAD INDEX !!!
            *pong1[i] = (uint16_t) ('\0');
        }

        pong1_y--;

        for(int i = 0; i < 6; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }

    else if(KeyInfo.scan_code == S_KEY)
    {
        if(pong1_y == 21)
            return;

        for(int i = 0; i < 6; i++)
        {
            //BAD INDEX !!!
            *pong1[i] = (uint16_t) ('\0');
        }

        pong1_y++;

        for(int i = 0; i < 6; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }

    else if(KeyInfo.scan_code == ARROW_UP || KeyInfo.character == 'p')
    {

        if(pong2_y == 1)
            return;

        for(int i = 0; i < 6; i++)
        {
            //BAD INDEX !!!
            *pong2[i] = (uint16_t) ('\0');
        }

        pong2_y--;

        for(int i = 0; i < 6; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }

    else if(KeyInfo.scan_code == ARROW_DOWN || KeyInfo.character == 'l')
    {
        if(pong2_y == 26)
            return;

        for(int i = 0; i < 6; i++)
        {
            *pong2[i] = (uint16_t) ('\0');
        }

        pong2_y++;

        for(int i = 0; i < 6; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }

    else if(KeyInfo.character == 'r')
    {
        pong_init();
    }

}




void pong_update(void)
{
    while(getscan() != ENTER)
    {
    
        msleep(75);
       
        *ball = '\0';
        ball = ball + ball_direction;

        
        ball = ball + (80 * ball_vector);


        if(*(ball + ball_direction) == (uint16_t)(' ' + (((lgray << 4) | lgray) << 8)))
        {
            ball_direction = ball_direction * -1;
            
        }


        if(rand() % 5 == 4)
            ball = ball + ball_direction;


        if(*(ball + 80) == (uint16_t)('\0' + (((lgreen << 4) | lgreen) << 8)))
        {
            ball_vector = ball_vector * -1;
        }

        if(*(ball - 80) == (uint16_t)('\0' + (((lgreen << 4) | lgreen) << 8)))
        {
            ball_vector = ball_vector * -1;
        }

        *ball = (' ' + (((white << 4) | white) << 8));


        if(*(ball + 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
        {
            pong_init();
        }

        if(*(ball - 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
        {  
            pong_init();
        }
    }
}



void pong(void)
{
    pong_init();
    pong_update();
}