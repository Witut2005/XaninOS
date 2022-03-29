
#include <lib/math.h>
#include <lib/string.h>
#include <lib/stdiox.h>
#include <lib/signal.h>
#include <lib/converters.h>
#include <headers/macros.h>



enum operation_types
{
    CALC_ADD = 1,
    CALC_SUB = 2,
    CALC_MUL = 3,
    CALC_DIV = 4,
    CALC_POW = 5
};

void calc()
{
    int operation_type;
    uint32_t a, b, result;

        character_blocked = ':';

    xprintf("choose type of operation:\n");
    xprintf("1. add\n");
    xprintf("2. subtract\n");
    xprintf("3. multiply\n");
    xprintf("4. divide\n");
    xprintf("5. powers\n");
    xprintf("6. convert dec to nBase number system\n");

    xscanf("%d", &operation_type);

        if(operation_type==6)
        {
                xprintf("\n base of number system: ");
                xscanf("%d", &a);

                 KeyInfo.scan_code = 0x0;

                    if(a > 35)
                        {
                   xprintf("\nnumber system not supported %zMAX 35%z", set_output_color(red,white));
                   goto end_run;
                        }

                xprintf("\n decimal number: ");
                xscanf("%d", &b);

                    result = d2nbns(a,b);

                xprintf("\noutput: %s", output_string);

                goto end_run;

        }

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
        case CALC_POW: result = pow(a,b); break;
        
    }

    xprintf("\nresult: %d", result);

    end_run:  while(getscan() != ENTER);
}

