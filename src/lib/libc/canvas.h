

#pragma once

char canvas_putchar(char character); // ONLY AVAILABLE IN CANVAS MODE
char canvas_putchar_color(uint8_t color, char character); // ONLY AVAILABLE IN CANVAS MODE

void canvas_xprintf(char* str, ... );
void canvas_xscanf(char* str, ... );
void canvas_xscan_range(char* string_buffer, uint32_t how_many_chars);
void canvas_screen_background_color_set(color_t color);