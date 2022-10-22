//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage

#include <stdint.h>
#include <libc/hal.h>
#include <libc/stdiox.h>
#include <terminal/vty.h>

uint8_t mouse_cycle=0;     //unsigned char
uint8_t mouse_uint8_t[3];    //signed char
int32_t mouse_x=0;         //signed char
int32_t mouse_y=0;         //signed char
uint8_t flip_flop = 0;
uint32_t packet_limiter_x = 0;
uint32_t packet_limiter_y = 0;

//Mouse functions
void my_mouse_handler(void) //struct regs *a_r (not used but just there)
{
 
	switch(mouse_cycle)
  	{
		case 0:
      		mouse_uint8_t[0]=inbIO(0x60);
      		mouse_cycle++;
      		break;
    	case 1:
      		mouse_uint8_t[1]=inbIO(0x60);
      		mouse_cycle++;
     	 	break;
    	case 2:
      		mouse_uint8_t[2]=inbIO(0x60);
			mouse_cycle=0;
			

				if(mouse_uint8_t[2] >= 2) 
				{
					if(packet_limiter_y < 1)
					{
						packet_limiter_y++;
						break;
					}

					packet_limiter_y =0 ;

					if((mouse_uint8_t[0] & 0x20))
						mouse_y++;
					else
						mouse_y--;
				}
				
				else
				{
					if(mouse_uint8_t[1] < 1)
						break;

					if(packet_limiter_x < 3)
					{
						packet_limiter_x++;
						break;
					}
					packet_limiter_x = 0;


					if((mouse_uint8_t[0] & 0x10)) // overflow x AHA
						mouse_x--;
					else
						mouse_x++;
				}
				// flip_flop = ~flip_flop;

			
			// if(mouse_uint8_t[0] & 0x40)
			// 	xprintf("0x%x\n", mouse_x);

			if(mouse_y < 0)
				mouse_y=0;
			if(mouse_x < 0)
				mouse_x = 0;

			if(mouse_y >= 27)
				mouse_y = 27;
			if(mouse_x >= 79)
				mouse_x = 79;

			// screen_clear();
			Screen.y = 0;
			for(int i = 3; i < 10; i++)
				Screen.cursor[Screen.y][i] = 0x0;
			xprintf("x: %d\n", mouse_x);

			for(int i = 3; i < 10; i++)
				Screen.cursor[Screen.y][i] = 0x0;
			xprintf("y: %d\n", mouse_y);

			// Screen.cursor[mouse_y][mouse_x]= (uint16_t)((uint8_t)Screen.cursor[mouse_y][mouse_x] | (0xF0 << 8));
			break;
  }

   eoi_send();

}

// void my_mouse_handler(void)
// {

//     xprintf("pilka nozna");
//     eoi_send();

// }

void mouse_wait(uint8_t a_type) //unsigned char
{
	uint32_t _time_out=100000; //unsigned int
	if(a_type==0)
	{
		while(_time_out--) //Data
		{
		if((inbIO(0x64) & 1)==1)
		{
			return;
		}
		}
		return;
	}
	else
	{
		while(_time_out--) //Signal
		{
		if((inbIO(0x64) & 2)==0)
		{
			return;
		}
		}
		return;
	}
}

void mouse_write(uint8_t a_write) //unsigned char
{
	//Wait to be able to send a command
	mouse_wait(1);
	//Tell the mouse we are sending a command
	outbIO(0x64, 0xD4);
	//Wait for the final part
	mouse_wait(1);
	//Finally write
	outbIO(0x60, a_write);
}


uint8_t mouse_read(void)
{
	//Get's response from mouse
	mouse_wait(0);
	return inbIO(0x60);
}

void mouse_install(void)
{
	uint8_t _status;  //unsigned char

	outbIO(0x60, 0xFF);
	mouse_read();

	//Enable the auxiliary mouse device
	mouse_wait(1);
	outbIO(0x64, 0xA8);
	
	//Enable the interrupts
	mouse_wait(1);
	outbIO(0x64, 0x20);
	mouse_wait(0);
	_status=(inbIO(0x60) | 2);
	_status &= ~(1 << 5);

	mouse_wait(1);
	outbIO(0x64, 0x60);
	mouse_wait(1);
	outbIO(0x60, _status);

	
	//Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();  //Acknowledge
	
	//Enable the mouse
	mouse_write(0xF4);
	mouse_read();  //Acknowledge

	mouse_write(0xF3);
	mouse_read();

    
}