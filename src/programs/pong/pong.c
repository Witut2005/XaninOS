

#include <programs/pong/pong.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/xaninGraphics/xaninGraphics.h>
#include <sys/input/input.h>

//CANVAS APP

void pong_get_input(void)
{
    if(KeyInfo.character == 'w' || KeyInfo.character == 's')
        player1_input = KeyInfo.character;

    else if(KeyInfo.scan_code == ARROW_UP || KeyInfo.character == 'p')
        player2_input = 'p';

    else if(KeyInfo.scan_code == ARROW_DOWN || KeyInfo.character == 'l')
        player2_input = 'l';
        
}

void player1_get_input(void)
{
    
    if(player1_input == 'w')
    {
        if(pong1_y == 1)
            return;

        for(int i = 0; i < PONG_SIZE_Y; i++)   
            *pong1[i] = (uint16_t) ('\0');
        
        pong1_y--;

        for(int i = 0; i < PONG_SIZE_Y; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }

    }

    else if(player1_input == 's')
    {
        if(pong1_y == VGA_HEIGHT - PONG_SIZE_Y - 1)
            return;

        for(int i = 0; i < PONG_SIZE_Y; i++)
            *pong1[i] = (uint16_t) ('\0');
        
        pong1_y++;

        for(int i = 0; i < PONG_SIZE_Y; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }
}


void player2_get_input(void)
{

    if(player2_input == 'p')
    {

        if(pong2_y == 1)
            return;

        for(int i = 0; i < PONG_SIZE_Y; i++)
            *pong2[i] = (uint16_t) ('\0');
        
        pong2_y--;

        for(int i = 0; i < PONG_SIZE_Y; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }
    
    else if(player2_input == 'l')
    {
        if(pong2_y == VGA_HEIGHT - PONG_SIZE_Y - 1)
            return;

        for(int i = 0; i < 6; i++)
            *pong2[i] = (uint16_t) ('\0');
        

        pong2_y++;

        for(int i = 0; i < 6; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));
        }
    }

}

void result_screen(char* str)
{

    player1_points = 0;
    player2_points = 0;

    player1_input = 0;
    player2_input = 0;

    screen_clear();
    Screen.x = 33;
    Screen.y = 12;
    xprintf("%s wins", str);
    
    while(KeyInfo.scan_code != ENTER);
}

void ball_update(void)
{
 
    *ball = '\0';
    ball = ball + ball_direction;     
    ball = ball + (80 * ball_vector);

    Screen.cursor[3][25] = (uint16_t)((player1_points + '0') + (((black << 4) | white) << 8));
    Screen.cursor[3][54] = (uint16_t)((player2_points + '0') + (((black << 4) | white) << 8));

    if(*(ball + ball_direction) == (uint16_t)(' ' + (((lgray << 4) | lgray) << 8)))
        ball_direction = ball_direction * -1;
        
    if(*ball == (uint16_t)(' ' + (((lgray << 4) | lgray) << 8)))
    {
        ball = ball - (1 * ball_direction);      
        ball_direction = ball_direction * -1; 
    }

    if(!(rand() % 4))
        ball = ball + ball_direction;


    if(*(ball + 80) == (uint16_t)('\0' + (((lgreen << 4) | lgreen) << 8)))
        ball_vector = ball_vector * - 1;
    
    if(*(ball - 80) == (uint16_t)('\0' + (((lgreen << 4) | lgreen) << 8)))
        ball_vector = ball_vector * - 1;

    *ball = (' ' + (((white << 4) | white) << 8));


    if(*(ball + 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
    {
        player1_points++;
        pong_init();
    }

    if(*(ball - 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
    {
        player2_points++;
        pong_init();
    }
}

void pong_init(void)
{


    player1_input = 0;
    player2_input = 0;

    screen_clear();

    draw_line_x(0,79,0,lgreen);
    draw_line_x(0,79,VGA_HEIGHT - 1,lgreen);

    draw_line_y(0,VGA_HEIGHT - 1,0,green);
    draw_line_y(0,VGA_HEIGHT - 1,79,green);

    keyboard_handle = pong_get_input;

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





void pong_update(void)
{

    pong_get_input();
    player1_get_input();
    player2_get_input();
    ball_update();

    msleep(75);

    if(KeyInfo.character == 'r')    
    {
        player1_points = 0;
        player2_points = 0;
        pong_init();
    }
        
}



int pong(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    pong_init();

    while(KeyInfo.scan_code != ESC && KeyInfo.scan_code != ENTER && KeyInfo.scan_code != F4_KEY && app_exited != true)
    { 
        pong_update();
        if(player1_points == 5)
        {
            result_screen("LEFT PLAYER");
            pong_init();
        }

        else if(player2_points == 5)
        {
            result_screen("RIGHT PLAYER");
            pong_init();
            
        }
    }
    return XANIN_OK;

}