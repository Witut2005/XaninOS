//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage

#include <stdint.h>

uint8_t mouse_cycle=0;     //unsigned char
uint8_t mouse_uint8_t[3];    //signed char
uint8_t mouse_x=0;         //signed char
uint8_t mouse_y=0;         //signed char

//Mouse functions
void mouse_handler2() //struct regs *a_r (not used but just there)
{
    /*
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
      mouse_x=mouse_uint8_t[1];
      mouse_y=mouse_uint8_t[2];
      mouse_cycle=0;
      break;
  }
    */
   

}

void my_mouse_handler()
{

    xprintf("pilka nozna");
    outbIO(0xA0, 0x20);
    outbIO(0x20, 0x20);


}

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

uint8_t mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inbIO(0x60);
}

void mouse_install()
{
  uint8_t _status;  //unsigned char

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
  mouse_read();

  mouse_write(0xFF);
  mouse_read();
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge


  outbIO(0xa1, 0xF8);
  outbIO(0x21, 0xEF);
    
    
}