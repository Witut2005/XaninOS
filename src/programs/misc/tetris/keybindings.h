#pragma once

#include <stdint.h>
#include <sys/input/input.h>
#include <lib/libc/colors.h>
#include <lib/screen/screen.h>
#include <sys/devices/keyboard/scan_codes.h>

uint16_t buf;
uint8_t colorTetris = 0x1;

uint16_t* petris_cursor = (uint16_t*)0xB8000;

void tetris_input()
{

    if (input_is_normal_key_pressed(KBP_A))
        petris_cursor--;

    if (input_is_normal_key_pressed(KBP_D))
        petris_cursor++;

    if (input_is_normal_key_pressed(KBP_0))
        colorTetris = white;

    if (input_is_normal_key_pressed(KBP_1))
        colorTetris = blue;

    if (input_is_normal_key_pressed(KBP_2))
        colorTetris = green;

    if (input_is_normal_key_pressed(KBP_3))
        colorTetris = cyan;

    if (input_is_normal_key_pressed(KBP_4))
        colorTetris = red;

    if (input_is_normal_key_pressed(KBP_5))
        colorTetris = brown;

    if (input_is_normal_key_pressed(KBP_6))
        colorTetris = lblue;

    if (input_is_normal_key_pressed(KBP_7))
        colorTetris = yellow;

    if (input_is_normal_key_pressed(KBP_8))
        colorTetris = magenta;

    if (input_is_normal_key_pressed(KBP_9))
        colorTetris = lgray;

    if (input_is_normal_key_pressed(KBP_P))
    {
        *petris_cursor = buf;
        while (input_is_normal_key_pressed(KBP_P))
            ;

        *petris_cursor = 0x0;
    }
}