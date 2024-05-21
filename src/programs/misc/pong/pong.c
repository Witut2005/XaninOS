
#include "./pong.h"
#include <lib/libc/canvas.h>
#include <lib/libc/pair.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <lib/xaninGraphics/xaninGraphics.h>
#include <sys/input/input.h>

// CANVAS_APP

#define PONG_POINTS_NEEDED_TO_WIN 5
#define PONG_SIZE_Y 6

#define PONG1_X 5
#define PONG2_X 74
#define PONGS_BASE_Y 10

#define PONG_MIN_Y_POSITION 1
#define PONG_MAX_Y_POSITION (VGA_HEIGHT - PONG_SIZE_Y - 1)

#define PONG_CELL ((uint16_t)(' ' + (((lgray << 4) | lgray) << 8)))
#define WALL_CELL ((uint16_t)('\0' + (((green << 4) | green) << 8)))

enum PONG_BALL_DIRECTIONS {
    BALL_DIRECTION_LEFT = -1,
    BALL_DIRECTION_RIGHT = 1,
    BALL_DIRECTION_UP = -1,
    BALL_DIRECTION_DOWN = 1,
};

#define BALL_VECTOR_X_GET(Pong) (Pong->BallVector.first)
#define BALL_VECTOR_Y_GET(Pong) (Pong->BallVector.second)
#define BALL_VECTOR_X_SET(Pong, val) (Pong->BallVector.first = val)
#define BALL_VECTOR_Y_SET(Pong, val) (Pong->BallVector.second = val)
#define BALL_VECTOR_X_REVERSE(Pong) (Pong->BallVector.first = Pong->BallVector.first * -1)
#define BALL_VECTOR_Y_REVERSE(Pong) (Pong->BallVector.second = Pong->BallVector.second * -1)

static inline void pong_get_input(KeyInfo key_info, PongState* pong)
{
    if (key_info.scan_code == KBP_W || key_info.scan_code == KBP_S)
        pong->PlayersInput.first = key_info.scan_code;

    else if (key_info.scan_code == KBP_P || key_info.scan_code == KBSP_ARROW_UP)
        pong->PlayersInput.second = key_info.scan_code;

    else if (key_info.scan_code == KBP_L || key_info.scan_code == KBSP_ARROW_DOWN)
        pong->PlayersInput.second = key_info.scan_code;
}

static inline void player1_position_update(PongState* Pong)
{

    uint8_t pong1_y_tmp = Pong->Positions.first;

    if (Pong->PlayersInput.first == KBP_W)
    {
        if (Pong->Positions.first == PONG_MIN_Y_POSITION)
            return;
        Pong->Positions.first--;
    }

    else if (Pong->PlayersInput.first == KBP_S)
    {
        if (Pong->Positions.first == PONG_MAX_Y_POSITION)
            return;
        Pong->Positions.first++;
    }

    for (int i = 0; i < PONG_SIZE_Y; i++)
        stdio_canvas_cell_put(BLANK_SCREEN_CELL, 0, pong1_y_tmp + i, PONG1_X);

    for (int i = 0; i < PONG_SIZE_Y; i++)
        stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), Pong->Positions.first + i, PONG1_X);
}

static inline void player2_position_update(PongState* Pong)
{
    uint8_t pong2_y_tmp = Pong->Positions.second;

    if ((Pong->PlayersInput.second == KBP_P) | (Pong->PlayersInput.second == KBSP_ARROW_UP))
    {
        if (Pong->Positions.second == PONG_MIN_Y_POSITION)
            return;
        Pong->Positions.second--;
    }

    else if ((Pong->PlayersInput.second == KBP_L) | (Pong->PlayersInput.second == KBSP_ARROW_DOWN))
    {
        if (Pong->Positions.second == PONG_MAX_Y_POSITION)
            return;
        Pong->Positions.second++;
    }

    for (int i = 0; i < PONG_SIZE_Y; i++)
        stdio_canvas_cell_put(BLANK_SCREEN_CELL, 0, pong2_y_tmp + i, PONG2_X);

    for (int i = 0; i < PONG_SIZE_Y; i++)
        stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), Pong->Positions.second + i, PONG2_X);
}

static inline void result_screen(char* str)
{
    canvas_screen_clear();
    stdio_canvas_move_to_x(33);
    stdio_canvas_move_to_y(12);

    canvas_xprintf("%s wins", str);

    while (!input_is_normal_key_pressed(KBP_ENTER))
        ;
}

static inline void pong_default_state_restore(PongState* Pong)
{
    canvas_screen_clear();

    draw_line_x(0, VGA_MAX_X, 0, lgreen);
    draw_line_x(0, VGA_MAX_X, VGA_MAX_Y, lgreen);

    draw_line_y(0, VGA_HEIGHT - 1, 0, green);
    draw_line_y(0, VGA_HEIGHT - 1, VGA_MAX_X, green);

    for (int i = 0; i < PONG_SIZE_Y; i++)
    {
        stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), PONGS_BASE_Y + i, PONG1_X);
        stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), PONGS_BASE_Y + i, PONG2_X);
    }

    Pong->PlayersInput.first = Pong->PlayersInput.second = 0;
    Pong->Positions.first = Pong->Positions.second = PONGS_BASE_Y;

    Pong->PlayersInput.first = Pong->PlayersInput.second = 0;
    Pong->Positions.first = Pong->Positions.second = PONGS_BASE_Y;

    Pong->Ball.x = 39;
    Pong->Ball.y = 10;

    stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), Pong->Ball.y, Pong->Ball.x);
}

static inline void ball_update(PongState* Pong)
{
    stdio_canvas_cell_put(BLANK_SCREEN_CELL, 0, Pong->Ball.y, Pong->Ball.x);

    Pong->Ball.x += BALL_VECTOR_X_GET(Pong);
    Pong->Ball.y += BALL_VECTOR_Y_GET(Pong);

    stdio_canvas_cell_put(Pong->PlayersPoints.first + '0', OUTPUT_COLOR_SET(black, white), 3, 25);
    stdio_canvas_cell_put(Pong->PlayersPoints.second + '0', OUTPUT_COLOR_SET(black, white), 3, 54);

    if (!(rand() % 4))
        Pong->Ball.x += BALL_VECTOR_X_GET(Pong);

    if (stdio_canvas_cell_background_color_compare(Pong->Ball.y, Pong->Ball.x, lgray) | stdio_canvas_cell_background_color_compare(Pong->Ball.y, Pong->Ball.x + BALL_VECTOR_X_GET(Pong), lgray))
    {
        BALL_VECTOR_X_REVERSE(Pong);
    }

    if (stdio_canvas_cell_background_color_compare(Pong->Ball.y + 1, Pong->Ball.x, lgreen) | stdio_canvas_cell_background_color_compare(Pong->Ball.y - 1, Pong->Ball.x, lgreen))
    {
        BALL_VECTOR_Y_REVERSE(Pong);
    }

    if (stdio_canvas_cell_background_color_compare(Pong->Ball.y, Pong->Ball.x, green) | stdio_canvas_cell_background_color_compare(Pong->Ball.y, Pong->Ball.x + BALL_VECTOR_X_GET(Pong), green))
    {
        if (BALL_VECTOR_X_GET(Pong) == BALL_DIRECTION_RIGHT)
            Pong->PlayersPoints.first++;
        else
            Pong->PlayersPoints.second++;
        pong_default_state_restore(Pong);
    }

    stdio_canvas_cell_put(' ', OUTPUT_COLOR_SET(lgray, lgray), Pong->Ball.y, Pong->Ball.x);
}

void pong_update(PongState* Pong)
{
    player1_position_update(Pong);
    player2_position_update(Pong);
    ball_update(Pong);

    msleep(75);

    if (input_is_normal_key_pressed(KBP_R))
    {
        Pong->PlayersPoints.first = Pong->PlayersPoints.second = 0;
        pong_default_state_restore(Pong);
    }
}

int pong(void)
{

    PongState pong = { { PONGS_BASE_Y, PONGS_BASE_Y }, { 0, 0 }, { 0, 0 }, { BALL_DIRECTION_RIGHT, BALL_DIRECTION_UP }, { 10, 39 } };

    stdio_mode_set(STDIO_MODE_CANVAS);
    pong_default_state_restore(&pong);

    InputHandler PongHandler = input_handler_create((input_handler_t)pong_get_input, input_handler_options_create(&pong, INPUT_USER));
    input_handler_add(PongHandler, INPUT_USER);

    while (!input_is_normal_key_pressed(KBP_F4))
    {
        pong_update(&pong);
        if (pong.PlayersPoints.first == PONG_POINTS_NEEDED_TO_WIN)
        {
            result_screen("LEFT PLAYER");
            pong_default_state_restore(&pong);
            pong.PlayersPoints.first = pong.PlayersPoints.second = 0;
        }

        else if (pong.PlayersPoints.second == PONG_POINTS_NEEDED_TO_WIN)
        {
            result_screen("RIGHT PLAYER");
            pong_default_state_restore(&pong);
            pong.PlayersPoints.first = pong.PlayersPoints.second = 0;
        }
    }
    return XANIN_OK;
}
