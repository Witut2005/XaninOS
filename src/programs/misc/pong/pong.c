
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/time.h>
#include <lib/xaninGraphics/xaninGraphics.h>
#include <sys/input/input.h>
#include <lib/screen/screen.h>
#include <lib/libc/canvas.h>

//CANVAS_APP

#define PONG_SIZE_Y 6

static uint16_t* pong1[6];
static uint16_t* pong2[6];

static uint8_t pong1_y = 0;
static uint8_t pong2_y = 0;

static uint16_t* ball;

static int ball_direction = 1;
static int ball_vector = 1;

static uint8_t player1_input = 0, player2_input = 0;
static uint8_t player1_points = 0, player2_points = 0;

static inline void pong_get_input(key_info_t KeyboardInfo, uint8_t** args)
{
    if((KeyboardInfo.scan_code == KBP_W) | (KeyboardInfo.scan_code == KBP_S))
        player1_input = KeyboardInfo.scan_code;

    else if((KeyboardInfo.scan_code == KBP_P) | (KeyboardInfo.scan_code == KBSP_ARROW_UP))
        player2_input = KeyboardInfo.scan_code;

    else if((KeyboardInfo.scan_code == KBP_L) | (KeyboardInfo.scan_code == KBSP_ARROW_DOWN))
        player2_input = KeyboardInfo.scan_code;
        
}

static inline void player1_position_update(void)
{
    if(player1_input == KBP_W)
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

    else if(player1_input == KBP_S)
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

static inline void player2_position_update(void)
{

    if((player2_input == KBP_P) | (player2_input == KBSP_ARROW_UP))
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
    
    else if((player2_input == KBP_L) | (player2_input == KBSP_ARROW_DOWN))
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

static inline void result_screen(char* str)
{
    player1_points = player2_points =  0;
    player1_input = player2_input = 0;

    canvas_screen_clear();
    stdio_canvas_move_to_x(33);
    stdio_canvas_move_to_y(12);

    canvas_xprintf("%s wins", str);
    
    while(!__input_is_normal_key_pressed(KBP_ENTER));
}

static inline void pong_default_state_restore(void)
{
    player1_input = player2_input = 0;

    canvas_screen_clear();

    draw_line_x(0,79,0,lgreen);
    draw_line_x(0,79,VGA_HEIGHT - 1,lgreen);

    draw_line_y(0,VGA_HEIGHT - 1,0,green);
    draw_line_y(0,VGA_HEIGHT - 1,79,green);

    for(int i = 0; i < 6; i++)
    {
        pong1[i] = &Screen.cursor[10 + i][5];
        *pong1[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));

        pong2[i] = &Screen.cursor[10 + i][74];
        *pong2[i] = (uint16_t) (' ' + (((lgray << 4) | lgray) << 8));

    }
    
    pong1_y = pong2_y = 10;
    ball = &Screen.cursor[10][39];

    *ball = (' ' + (((white << 4) | white) << 8));
}


static inline void ball_update(void)
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
    
    else if(*(ball - 80) == (uint16_t)('\0' + (((lgreen << 4) | lgreen) << 8)))
        ball_vector = ball_vector * - 1;

    *ball = (' ' + (((white << 4) | white) << 8));

    if(*(ball + 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
    {
        player1_points++;
        pong_default_state_restore();
    }

    else if(*(ball - 1) == (uint16_t)('\0' + (((green << 4) | green) << 8)))
    {
        player2_points++;
        pong_default_state_restore();
    }
}

static inline void players_position_update(void)
{
    player1_position_update();
    player2_position_update();
}

void pong_update(void)
{
    players_position_update();
    ball_update();

    msleep(75);

    if(__input_is_normal_key_pressed(KBP_R))    
    {
        player1_points = 0;
        player2_points = 0;
        pong_default_state_restore();
    }
}

int pong(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    pong_default_state_restore();

    InputHandler PongHandler = input_handler_create(pong_get_input, input_handler_options_create(NULL, USER_INPUT_HANDLER));
    __input_add_handler(&PongHandler);

    while(!__input_is_normal_key_pressed(KBP_F4))
    { 
        pong_update();
        if(player1_points == 5)
        {
            result_screen("LEFT PLAYER");
            pong_default_state_restore();
        }

        else if(player2_points == 5)
        {
            result_screen("RIGHT PLAYER");
            pong_default_state_restore();
        }
    }
    return XANIN_OK;
}