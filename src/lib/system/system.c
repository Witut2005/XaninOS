#include "./system.h"
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>

#warning "TODO put every __sys like function to this folder";

char* system_variable_get(char* variable)
{
    XinEntry* File = fopen("/etc/var/variables.conf", "r");
    char* line = getline(File, 0);
    uint32_t var_len = strlen(variable);

    for (int i = 0; line != NULL;)
    {
        if (bstrncmp(line, variable, var_len - 1))
        {
            strcpy(line, &line[var_len + 1]);
            return line;
        }
        line = getline(File, ++i);
    }

    fclose(&File);
    free(line);
    return NULL;
}

uint32_t xanin_syscall0(uint32_t syscall_id)
{
    return xanin_syscall4(syscall_id, 0, 0, 0, 0);
}

uint32_t xanin_syscall1(uint32_t syscall_id, uint32_t arg1)
{
    return xanin_syscall4(syscall_id, arg1, 0, 0, 0);
}

uint32_t xanin_syscall2(uint32_t syscall_id, uint32_t arg1, uint32_t arg2)
{
    return xanin_syscall4(syscall_id, arg1, arg2, 0, 0);
}

uint32_t xanin_syscall3(uint32_t syscall_id, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    return xanin_syscall4(syscall_id, arg1, arg2, arg3, 0);
}

uint32_t xanin_syscall4(uint32_t syscall_id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
    uint32_t ret;
    // Wywołanie syscalla za pomocą inline assemblera w składni Intel
    asm volatile(
        "mov eax, %1\n\t"                                  // Przypisanie numeru syscall do rejestru EAX
        "mov ecx, %2\n\t"                                  // Pierwszy argument do rejestru ECX
        "mov edx, %3\n\t"                                  // Drugi argument do rejestru EDX
        "mov ebx, %4\n\t"                                  // Trzeci argument do rejestru EBX
        "int 0x81\n\t"                                     // Wywołanie syscalla
        "mov %0, eax"                                      // Zapisanie wyniku syscalla
        : "=r"(ret)                                        // Wyjściowy argument (wynik syscalla) - zapisany w ret
        : "g"(syscall_id), "g"(arg1), "g"(arg2), "g"(arg3) //, "g"(arg4) // Wejściowe argumenty
        : "eax", "ecx", "edx", "ebx"                       // Rejestry używane przez syscall
    );
    return ret; // Zwracanie wyniku syscalla
}
