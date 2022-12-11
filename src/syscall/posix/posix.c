

#include <stdint.h>
#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <app_config.h>
#include <syscall/posix/syscall_number.h>
#include <libc/stdlibx.h>
#include <libc/time.h>
#include <libc/hal.h>
#include <libc/memory.h>
#include <libc/string.h>


uint32_t syscall_handle(void)
{
   

    uint32_t eax, ecx, edx, ebx, esi, edi;

    asm(
        "mov %0, eax;"     
        "mov %1, ecx;"     
        "mov %2, edx;"     
        "mov %3, ebx;"     
  
        : "=g"(eax), "=g"(ecx), "=g"(edx), "=g"(ebx)
        :
        : "ecx", "edx", "ebx"
    );

    interrupt_enable();
   
    switch(eax)
    {

        case __NR_exit:
        {
            last_command_exit_status = ebx;
            exit();
            break;
        }

        case __NR_read: 
        {
            if(!FileDescriptorTable[ebx].is_used)
                xscan_range(ecx, edx);

            else
                eax = read(ebx, (void*)ecx, edx);

            break;
        }

        case __NR_write:
        {
            int how_many_writed;
            if(!FileDescriptorTable[ebx].is_used)
            {
                for(int i = 0; i < edx; i++)
                    xprintf("%c", *(uint8_t*)(ecx + i));    
            }
            
            else
                eax = write(ebx, (void*)ecx, edx);

            break;
        }
        
        case __NR_open:
        {                

            eax = open((char*)ebx, ecx); //edx = mode
            break;
        }
        
        case __NR_close:
        {
            close(ebx);
            break;
        }
        
        case __NR_mkdir:   
        {
            char* folder_name = (char*)ebx;
            bool allocated_additional_memory = false;
            
            if(folder_name[strlen(folder_name) - 1] != '/')
            {
                folder_name = (char*)calloc(strlen(folder_name) + 1);
                memcpy(folder_name, (uint8_t*)ebx, strlen((char*)ebx));
                folder_name[strlen(folder_name)] = '/';
                allocated_additional_memory = true;
            }

            __sys_xin_folder_create((char*)folder_name);

            if(allocated_additional_memory)
                free(folder_name);

            break;
        }

        case __NR_rmdir:
        {
            __sys_xin_folder_remove((char*)ebx);
            break;
        }    

        case __NR_creat:
        {
            eax = __sys_xin_file_create((char*)ebx);
        }

        case 100:
        {
            msleep(5000);
            break;
        }


    }

    interrupt_disable();
    return eax;

}

