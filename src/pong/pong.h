
#pragma once

#include <stdint.h>
#include <lib/signal.h>
#include <headers/colors.h>
#include <lib/stdlibx.h>
#include <lib/stdiox.h>

uint16_t* pongCur[8];

void createRectangle(uint8_t xSize, uint8_t ySize,uint8_t xwhere,uint8_t ywhere,uint8_t id);
void update(void);
void drawLineX(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor);
void drawLineY(uint8_t starty, uint8_t endy,uint8_t startx,uint8_t lineColor);
void keyControlPong(void);
void initPong(void);
void updatePong(void);


uint16_t figureField;

uint32_t* pixels1[20]; // must be higher 
uint32_t* pixels2[20]; // must be higher 

uint8_t player1Buf;
uint8_t player2Buf;

uint8_t player1[3] = {'w','s','d'};
uint8_t player2[3] = {'8','2','4'};

uint16_t* ball;

uint8_t vector;
    
int16_t velocity = 1;

struct destination
{

    bool changeDirection:1;

}dest;





