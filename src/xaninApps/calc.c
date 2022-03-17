
#include <lib/math.h>
#include <lib/string.h>
#include <lib/stdiox.h>
#include <lib/signal.h>
#include <headers/macros.h>


enum operation_types
{
    CALC_ADD = 1,
    CALC_SUB = 2,
    CALC_MUL = 3,
    CALC_DIV = 4
};

void calc()
{

    int operation_type;
    uint32_t a, b, result;

    xprintf("choose type of operation:\n");
    xprintf("1. add\n");
    xprintf("2. subtract\n");
    xprintf("3. multiply\n");
    xprintf("4. divide\n");

    xscanf("%d", &operation_type);

    xprintf("\nfirst number: ");
    xscanf("%d", &a);

    KeyInfo.scan_code = 0x0;

    xprintf("\nsecond number: ");
    xscanf("%d", &b);
    

    switch(operation_type)
    {
        case CALC_ADD: result = a + b; break;
        case CALC_SUB: result = a - b; break;
        case CALC_MUL: result = a * b; break;
        case CALC_DIV: result = a / b; break;
        
    }

    xprintf("\nyour result: %d", result);

    while(getscan() != ENTER);

}

