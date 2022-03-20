

#include <xaninApps/pong/pong.h>
#include <lib/stdlibx.h>
#include <lib/stdiox.h>

void pong_input(void)
{   

    if(getscan() == W_KEY)
    {
        for(int i = 0; i < 6; i++)
        {
            //ZLY INDEX !!!
            *pong1[i] = (uint16_t) ('\0');
        }

        pong1_y--;

        for(int i = 0; i < 6; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));
        }
    }

    else if(getscan() == S_KEY)
    {

        for(int i = 0; i < 6; i++)
        {
            //ZLY INDEX !!!
            *pong1[i] = (uint16_t) ('\0');
        }

        pong1_y++;

        for(int i = 0; i < 6; i++)
        {
            pong1[i] = &Screen.cursor[pong1_y + i][5];
            *pong1[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));
        }
    }

    else if(getscan() == O_KEY)
    {
        for(int i = 0; i < 6; i++)
        {
            //ZLY INDEX !!!
            *pong2[i] = (uint16_t) ('\0');
        }

        pong2_y--;

        for(int i = 0; i < 6; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));
        }
    }

    else if(getscan() == J_KEY)
    {

        for(int i = 0; i < 6; i++)
        {
            //ZLY INDEX !!!
            *pong2[i] = (uint16_t) ('\0');
        }

        pong2_y++;

        for(int i = 0; i < 6; i++)
        {
            pong2[i] = &Screen.cursor[pong2_y + i][74];
            *pong2[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));
        }
    }

}




void pong_update(void)
{
    while(getscan() != ENTER);
}

void pong_init(void)
{

    screen_clear();

    keyboard_handle = pong_input;

    for(int i = 0; i < 6; i++)
    {
        pong1[i] = &Screen.cursor[10 + i][5];
        *pong1[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));

        pong2[i] = &Screen.cursor[10 + i][74];
        *pong2[i] = (uint16_t) (' ' + (((white << 4) | white) << 8));

    }
    
    pong1_y = 10;
    pong2_y = 10;

    pong_update();

}