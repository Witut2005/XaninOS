

#pragma once

#include "../headers/inttypes.h"

//http://www.osdever.net/tutorials/view/how-to-program-the-dma


typedef struct
{

    uint8_t segment;
    uint32_t offset;
    uint32_t segSelector;

}dmaStruct;


void loadDmaData(dmaStruct *block,uint8_t data);


//MACROS



//DMA PORTS



//DMA_MODE_REGISTER

//BITS 1-0
#define CHANNEL_0_SELECT    0x00
#define CHANNEL_1_SELECT    0x01
#define CHANNEL_2_SELECT    0x02
#define CHANNEL_3_SELECT    0x03

//BITS 3-2
#define VERIFY_TRANSFER     0x00
#define WRITE_TRANSER       0x04
#define READ_TRANSFER       0x08
#define ILLEGAL_COMBINATION 0x0C

//BIT 4
#define AUTOINIT_DISABLE    0x00
#define AUTOINIT_ENABLE     0x10

//BIT 5
#define ADDRES_INC          0x00
#define ADDRES_DEC          0x20

//BITS 7-6
#define DEMAND_MODE         0x00
#define SINGLE_MODE         0x40
#define BLOCK_MODE          0x80
#define CASCADE_MODE        0xC0


