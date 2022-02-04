#pragma once

#include <headers/colors.h>
#include <xaninGraphics/xaninGraphics.h>


void create_rectangle(uint8_t x_size, uint8_t y_size, uint8_t x_position, uint8_t y_position);

/*

uint32_t* pixels1[4];
uint32_t* pixels2[4];

char player1_key[] = {'w','d'};
char player1_key[] = {'8','4'};

*/

void update_screen(void);
void pong_app_init(void);


struct ball_info
{

    uint16_t* ball_pointer;
    uint8_t velocity;
    uint8_t ball_destination;

}ball_t;





