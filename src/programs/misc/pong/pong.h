
#pragma once

#include <lib/libc/pair.h>
#include <stdint.h>

struct BallInfo {
    int8_t y;
    int8_t x;
};

typedef struct BallInfo BallInfo;

struct PongState {
    PairUInt8 Positions;

    PairUInt8 PlayersInput;
    PairUInt8 PlayersPoints;

    PairInt8 BallVector;
    BallInfo Ball;
};

typedef struct PongState PongState;

// static inline void pong_get_input(KeyInfo KeyboardInfo, uint8_t **args);
// static inline void player1_position_update(PongState *);
// static inline void player2_position_update(PongState *);
// static inline void result_screen(char *str);
// static inline void pong_default_state_restore(PongState *);
// static inline void ball_update(PongState *);
// void pong_update(PongState *);
// int pong(void);
