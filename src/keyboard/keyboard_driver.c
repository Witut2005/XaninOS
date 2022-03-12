
#include <terminal/vty.h>

void keyboard_driver(uint8_t scanCode)
{

    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

    KeyInfo.scan_code = scanCode;


    switch(KeyInfo.scan_code)
    {
        case LSHIFT: {KeyInfo.is_shift = true; return;}
        case LSHIFT_RELEASE: {KeyInfo.is_shift = false; return;}
        case BSPC: 
        {
            
            

            KeyInfo.is_bspc = true; 
            
            if((char)Screen.cursor[Screen.y][Screen.x - 1] == character_blocked)
            {
                return;
            }

            Screen.x--;

            if(!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
            }

           if(index)
                index--;

            comBuf[index] = '\0';
            Screen.cursor[Screen.y][Screen.x] = '\0';

            
            return;
        }
        
        case BSPC_RELEASE: {KeyInfo.is_bspc = false; return;}
        case CAPS: 
        {
            KeyInfo.is_caps = KeyInfo.is_caps  ?  false : true; 
            return; 
        }

    
    }

    KeyInfo.character = keyboard_map[scanCode];


    if((KeyInfo.is_caps) || (KeyInfo.is_shift))
    {
        if(KeyInfo.character >= 'a' && KeyInfo.character <= 'z')
        {
            KeyInfo.character -= 32;
            return;
        }
    }

    if(KeyInfo.scan_code >= 128)
    {
        
    }

    if(KeyInfo.is_shift)
    {
    key_remap('-','_');
    key_remap('1','!');
    key_remap('2', '@');
    key_remap('3','#');
    key_remap('4', '$');
    key_remap('5', '%');
    key_remap('6', '^');
    key_remap('7', '&');
    key_remap('8', '*');
    key_remap('9','(');
    key_remap('0',')');
    key_remap('=', '+');
    key_remap('[', '{');
    key_remap(']', '}');
    key_remap('/', '?');
    key_remap(';', ':');
    key_remap('`','~');
    key_remap(',','<');
    key_remap('.', '>');

    }

    /*

    if(KeyInfo.scan_code >= 128 )
    {
        KeyInfo.character = '\0';
        KeyInfo.scan_code = 0x0;
        return;
    }

    */

    //xprintf("%c", KeyInfo.character);
          
    
}