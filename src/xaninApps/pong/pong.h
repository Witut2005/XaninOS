

#include <stdint.h>


uint16_t* pong1[6];
uint16_t* pong2[6];

uint8_t pong1_y = 0;
uint8_t pong2_y = 0;

uint16_t* ball;

int ball_direction = 1;
int ball_vector = 1;

void pong_init(void);
void pong_update(void);
void pong_input(void);
void pong(void);