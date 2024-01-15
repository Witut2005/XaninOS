#pragma once

#include <stdint.h>
#include <sys/input/input.h>
#include <lib/libc/colors.h>
#include <lib/screen/screen.h>
#include <sys/devices/keyboard/scan_codes.h>

uint16_t buf;
uint8_t colorTetris = 0x1;

uint16_t *petris_cursor = (uint16_t *)0xB8000;

void tetris_input()
{

    if (__input_is_normal_key_pressed(KBP_A))
        petris_cursor--;

    if (__input_is_normal_key_pressed(KBP_D))
        petris_cursor++;

    if (__input_is_normal_key_pressed(KBP_0))
        colorTetris = white;

    if (__input_is_normal_key_pressed(KBP_1))
        colorTetris = blue;

    if (__input_is_normal_key_pressed(KBP_2))
        colorTetris = green;

    if (__input_is_normal_key_pressed(KBP_3))
        colorTetris = cyan;

    if (__input_is_normal_key_pressed(KBP_4))
        colorTetris = red;

    if (__input_is_normal_key_pressed(KBP_5))
        colorTetris = brown;

    if (__input_is_normal_key_pressed(KBP_6))
        colorTetris = lblue;

    if (__input_is_normal_key_pressed(KBP_7))
        colorTetris = yellow;

    if (__input_is_normal_key_pressed(KBP_8))
        colorTetris = magenta;

    if (__input_is_normal_key_pressed(KBP_9))
        colorTetris = lgray;

    if (__input_is_normal_key_pressed(KBP_P))
    {
        *petris_cursor = buf;
        while (__input_is_normal_key_pressed(KBP_P))
            ;

        *petris_cursor = 0x0;
    }
}