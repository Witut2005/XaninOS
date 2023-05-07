
#include <lib/libc/math.h>
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
 
enum operation_types
{
    CALC_ADD = 1,
    CALC_SUB = 2,
    CALC_MUL = 3,
    CALC_DIV = 4,
    CALC_POW = 5
};

int calc(char* options)
{

    if(strcmp(options, "help"))
    {
        screen_clear();
        xprintf("xaninOS developer tools\n");
        xprintf("Simple calculator :))\n");
    	while(inputg().scan_code != ENTER);
        return XANIN_OK;
    }

    int operation_type;
    uint32_t a, b, result;


    xprintf("choose type of operation:\n");
    xprintf("1. add\n");
    xprintf("2. subtract\n");
    xprintf("3. multiply\n");
    xprintf("4. divide\n");
    xprintf("5. powers\n");

    xscanf("%d", &operation_type);


    xprintf("\nfirst number: ");
    xscanf("%d", &a);

    xprintf("\nsecond number: ");
    xscanf("%d", &b);
    

    switch(operation_type)
    {
        case CALC_ADD: result = a + b; break;
        case CALC_SUB: result = a - b; break;
        case CALC_MUL: result = a * b; break;
        case CALC_DIV: result = a / b; break;
        case CALC_POW: result = pow(a,b); break;
        
    }

    xprintf("\nresult: %d", result);

    while(getscan() != ENTER);
    return XANIN_OK;
}

