#include "../lib/math.h"
#include "../lib/string.h"
#include "../lib/stdiox.h"
#include "../lib/signal.h"
#include "../headers/macros.h"

static uint8_t calcCom[11];
static uint8_t indexCalc = 0;


void calcKeyboard()
{
    calcCom[indexCalc] = input;
    indexCalc++;

}

void calc()
{

    KEYBOARD_SIG_ON(calcKeyboard);


    uint32_t toRet;

    uint16_t* calcCur = 0xb8000 + (80 * 5);

    sprint(black,white,"type of operation: ");
    sprint(black,white,"1. add");
    sprint(black,white,"2. sub");



    reverseStr(numStr);
    toRet = atoi(numStr);



}
