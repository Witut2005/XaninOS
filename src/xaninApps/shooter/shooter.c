
#include <headers/colors.h>
#include <xaninApps/shooter/shooter.h>
#include <lib/stdiox.h>


void shooter_init(void)
{

    

    for(int i = 0; i < 3; i++)
    {
        player1[i] = (uint32_t*)&Screen.cursor[10 + i][10];
        player2[i] = (uint32_t*)&Screen.cursor[10 + i][69];

        *player1[i] = white32;
        *player2[i] = white32;
    }

    while(1);

}


void shooter(void)
{
    screen_clear();
    shooter_init();
}