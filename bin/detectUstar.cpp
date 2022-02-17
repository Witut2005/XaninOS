
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <algorithm>
#include <cstring>

using namespace std;


#define NOP 0x90

fstream file;
bool ustar = false;
static uint8_t buf[0x30 * 512];
char* buf_ptr = (char*)buf;
uint64_t fileSize;
uint32_t offset;

char entries_counter = 0x0;


void find_ustar(char* x)
{
    for(int i = 0; i < 50 * 512 - 1; i++)
    {
        if(strcmp(&x[i],"ustar  ") == 0)
            entries_counter++;
    }

}

int main(void)
{

    printf("\n*********************************\n");
    printf ("TOOLS FOR XANIN OPERATING SYSTEM\n");
    printf("*********************************\n");

    file.open("xanin.img",ios::binary | ios::in | ios::out); 



    //GET FILE SIZE
    file.seekg(0,ios::end);
    fileSize = file.tellg();
    file.seekg(0,ios::beg);


    file.read((char*)buf,fileSize);

    for(int i = 0; i < fileSize; i++)
    {



        if(buf[i] == NOP)
        {
            for(int j = 0; j < 16; j++)
            {
                if(buf[i + j] == NOP)
                    ustar = true;

                else 
                {
                    ustar = false;
                    break;
                }  

            }


        }

        if(ustar)
        {
            offset = i;
            break;
        }

    }

    printf("\nUSTAR FILE SYSTEM DETECTED IN OS IMAGE AT OFFSET 0x%x\n",offset + 0x10);
    offset = offset + 0x20000 - 0x200 + 0x10;   //fs start address is relative value so we need to add some addreses
    printf("USTAR FILE SYSTEM DETECTED AT OFFSET 0x%x\n\n",offset);



    file.seekg(5,ios::beg);

    file.write((char*)&offset,sizeof(uint32_t));

    file.close();

    file.open("xanin.img",ios::binary | ios::in | ios::out); 
    file.seekg(0,ios::beg);

    file.read((char*)buf,fileSize);


    find_ustar((char*)buf);
    file.seekg(0xd,ios::beg);

    printf("XANIN HAS %d FILE SYSTEM ENTRIES\n",entries_counter);

    file << entries_counter;

    file.close();

    file.open("xanin.img", ios::out | ios::app); 


    append:

    while((file.tellg() % 512) != 0)
        file << 0;    


    for(int i = 0; i < 512; i++)
        file << "test";

    file.close();

}
