
                    /* RESOURCES */
/*-------------------------------------------------------/
|   http://www.brokenthorn.com/Resources/OSDev21.html    |
|   https://wiki.osdev.org/ISA_DMA#Technical_Details     |
|   https://en.wikipedia.org/wiki/Intel_8237             |
/-------------------------------------------------------*/      

#pragma once

#include <libc/hal.h>
#include <stdint.h>
#include <libc/stdiox.h>
#include <devices/DMA/dma.h>


/* if you use dma1 channels you must sub 4 from channel_number */
static inline uint8_t dma_get_relative_channel(uint8_t dma_number, uint8_t channel)
{
    return channel = (dma_number == 1) ? channel - 4 : channel;
}

static inline void dma_controller_reset(void)
{
    outbIO(0xD,0x20);
    outbIO(0xD8,0x20);
}

/* channel_number, 16bit addres */
void dma_address_set(uint8_t channel_number, uint8_t address_low, 
                        uint8_t address_high)
{

    uint8_t port;

    if(channel_number > 8)
    {
        xprintf("%zINVALID DMAC CHANNEL NUMBER\n",set_output_color(red,white));
        return;
    }

	switch (channel_number) 
    {
		case 0: {port = DMA_SLAVE_CHAN0_ADDRESS_REG; break;}
		case 1: {port = DMA_SLAVE_CHAN1_ADDRESS_REG; break;}
		case 2: {port = DMA_SLAVE_CHAN2_ADDRESS_REG; break;}
		case 3: {port = DMA_SLAVE_CHAN3_ADDRESS_REG; break;}
		case 4: {port = DMA_MASTER_CHAN4_ADDRESS_REG; break;}
		case 5: {port = DMA_MASTER_CHAN5_ADDRESS_REG; break;}
		case 6: {port = DMA_MASTER_CHAN6_ADDRESS_REG; break;}
		case 7: {port = DMA_MASTER_CHAN7_ADDRESS_REG; break;}
	}

    outbIO(port, address_low);
    outbIO(port, address_high);
    

}   


void dma_count_set(uint8_t channel_number, uint8_t address_low, 
                    uint8_t address_high)
{
    if(channel_number > 8)
    {
        xprintf("%zINVALID DMAC CHANNEL NUMBER\n",set_output_color(red,white));
        return;
    }


	uint8_t port;

	switch (channel_number) 
    {

		case 0: {port = DMA_SLAVE_CHAN0_COUNT_REG; break;}
		case 1: {port = DMA_SLAVE_CHAN1_COUNT_REG; break;}
		case 2: {port = DMA_SLAVE_CHAN2_COUNT_REG; break;}
		case 3: {port = DMA_SLAVE_CHAN3_COUNT_REG; break;}
		case 4: {port = DMA_MASTER_CHAN4_COUNT_REG; break;}
		case 5: {port = DMA_MASTER_CHAN5_COUNT_REG; break;}
		case 6: {port = DMA_MASTER_CHAN6_COUNT_REG; break;}
		case 7: {port = DMA_MASTER_CHAN7_COUNT_REG; break;}
	}

	outbIO(port, address_low);
	outbIO(port, address_high);

}

void dma_mask_channel(uint8_t dma_number, uint8_t realtive_channel)
{

    /* you cannot set or clear mask bit, you can only negate its state */
    if(dma_number)
        outbIO(DMA_MASTER_CHANGE_MASK_REGISTER, realtive_channel);
    else
        outbIO(DMA_SLAVE_CHANGE_MASK_REGISTER, realtive_channel);
}

void dma_unmask_channel(uint8_t dma_number, uint8_t realtive_channel)
{

    /* you cannot set or clear mask bit, you can only negate its state */
    if(dma_number)
        outbIO(DMA_MASTER_CHANGE_MASK_REGISTER, realtive_channel);
    else
        outbIO(DMA_SLAVE_CHANGE_MASK_REGISTER, realtive_channel);
}

void dma_send_illegal_command(void)
{

}

void dma_channel_mode_set(uint8_t channel, uint8_t channel_mode)
{

    uint8_t dma_number = (channel < 4) ? DMA_SLAVE : DMA_MASTER;
    uint8_t relative_channel = dma_get_relative_channel(dma_number, channel);

    dma_mask_channel(dma_number, relative_channel);
    outbIO(channel,channel_mode);
    dma_unmask_channel(dma_number, relative_channel);
}

