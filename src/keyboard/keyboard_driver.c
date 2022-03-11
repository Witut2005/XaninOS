
#include <terminal/vty.h>

void keyboard_driver(uint8_t scanCode)
{


    key_info.character = 0x0;
    key_info.scan_code = 0x0;

    key_info.scan_code = scanCode;


    switch(key_info.scan_code)
    {
        case LSHIFT: {key_info.is_shift = true; return;}
        case LSHIFT_RELEASE: {key_info.is_shift = false; return;}
        case BSPC: 
        {
            if(key_info.is_bspc)
            {
                key_info.is_bspc = false;
                return;
            }
            key_info.is_bspc = true; 
            return;
        }
        
        case BSPC_RELEASE: {key_info.is_bspc = false; return;}
        case CAPS: {key_info.is_caps = ~key_info.is_caps; return;}
    }

    key_info.character = keyboard_map[scanCode];


    if((key_info.is_caps) || (key_info.is_caps))
        if(key_info.character >= 'a' && key_info.character <= 'z')
            key_info.character -= 32;

    if(key_info.is_shift)
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


    if(key_info.scan_code >= 128 )
    {
        key_info.character = '\0';
        key_info.scan_code = 0x0;
        return;
    }

    //xprintf("%c", key_info.character);
          
    
}