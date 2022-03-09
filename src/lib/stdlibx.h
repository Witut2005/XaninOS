
#pragma once

#include <lib/hal.h>
#include <pit/pit.c>
#include <terminal/vty.h>
#include <headers/colors.h>


struct TIME
{
    uint8_t seconds; 
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekDay;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t year;
    uint8_t century;
    

}__attribute__((packed));

struct TIME time;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

char* weekDaysLUT[7] = {"Sunday","Monday","Tuesday ","Wednesday","Thursday","Friday","Saturday"};

bool key_pressed(void)
{
    if(key_released)
        return false;
    else
        return true;
}

char getchar(void)
{
    keyboard_input = 0x0;
    while((!keyboard_input) || (keyboard_scan_code >= 128 ));
    return keyboard_input;
}

void getTime()
{
    asm("cli");


    //GET SECONDS
    outbIO(CMOS_ADDR,0x0);
    time.seconds = inbIO(CMOS_DATA);

    //GET MINUTES
    outbIO(CMOS_ADDR,0x2);
    time.minutes = inbIO(CMOS_DATA);

    //GET HOURS
    outbIO(CMOS_ADDR,0x4);
    time.hour = inbIO(CMOS_DATA);

    //GET WEEKDAY
    outbIO(CMOS_ADDR,0x6);
    time.weekDay = inbIO(CMOS_DATA);
    time.weekDay--;

    //GET DAY_OF_MONTH
    outbIO(CMOS_ADDR,0x7);
    time.dayOfMonth = inbIO(CMOS_DATA);

    //GET MONTH
    outbIO(CMOS_ADDR,0x8);
    time.month = inbIO(CMOS_DATA);

    //GET YEAR
    outbIO(CMOS_ADDR,0x9);
    time.year = inbIO(CMOS_DATA);

    //GET CENTURY
    outbIO(CMOS_ADDR,0x32);
    time.century = inbIO(CMOS_DATA);





    asm("sti");
}



void getCpuSpeed()
{
    set_pit_divisor(0x0);

    while(!pitActive)
        cpu_mhz++;

    pitActive = false;
}

static uint32_t randomizer;


//https://crypto.stackexchange.com/questions/6760/how-does-the-rand-function-in-c-work

void srand(uint32_t seed)
{
    randomizer = seed;
}


uint32_t rand()
{
    return (((randomizer = randomizer * 214013 + 2531011) >> 16) & 0x7fff);
}