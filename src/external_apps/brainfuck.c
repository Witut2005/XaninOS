

// ORIGINAL CODE !!!!!
// https://github.com/flodt/brainfuck/blob/master/brainfuck.c

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>

#define BUFFER_SIZE 32768

char* program;
char* buffer;
size_t pc, ptr;
size_t prog_len;

void move_ptr_fwd();

void move_ptr_bwd();

void incr_byte();

void decr_byte();

void print_char();

void read_char();

void jmp_fwd();

void jmp_bwd();

int main(int argc, char* argv[])
{

    // stdio_mode_set(STDIO_MODE_CANVAS);
    // __sys_vty_set(__sys_vty_get());

    // stdio_mode_set(STDIO_MODE_TERMINAL);
    xprintf("Brainfuck\n=========\n");

    if (argc < 2)
    {
        xprintf("Bad. No program specified.");
        return 1;
    }

    // grab our program
    program = argv[1];

    // initialize memory for the array
    buffer = calloc(strlen(program));
    if (buffer == NULL)
    {
        xprintf("Memory allocation fault.");
        return 2;
    }

    // now we can start translating the string
    pc = 0;
    ptr = 0;
    prog_len = strlen(program);

    // while we have not reached the end of the string
    while (pc != prog_len)
    {
        // parse the current instruction
        char current = program[pc];

        switch (current)
        {
        case '>':
            move_ptr_fwd();
            break;
        case '<':
            move_ptr_bwd();
            break;
        case '+':
            incr_byte();
            break;
        case '-':
            decr_byte();
            break;
        case '.':
            print_char();
            break;
        case ',':
            read_char();
            break;
        case '[':
            jmp_fwd();
            break;
        case ']':
            jmp_bwd();
            break;
        default:
            xprintf("Bad. Illegal character in program string:");//  %c", current);
            // asm volatile("jmp $");
            return 3;
        }
    }

    xprintf("\nOK\n");
    return 0;
}

void jmp_bwd()
{
    if (buffer[ptr] == 0)
    {
        pc++;
        return;
    }

    // jump backwards to the next fitting "["
    int depth = 0;
    for (int i = pc; i >= 0; --i)
    {
        switch (program[i])
        {
        case '[':
            depth--;
            break;
        case ']':
            depth++;
            break;
        default:
            break;
        }

        if (depth == 0)
        {
            pc = i;
            return;
        }
    }

    // we did not find a match, so the program is malformed
    xprintf("Illegal program string.");
    // exit(5);
    exit();
}

void jmp_fwd()
{
    if (buffer[ptr] != 0)
    {
        pc++;
        return;
    }

    // we need to jump forward to the next fitting "]"
    int depth = 0;
    for (int i = pc; i < prog_len; ++i)
    {
        switch (program[i])
        {
        case '[':
            depth++;
            break;
        case ']':
            depth--;
            break;
        default:
            break;
        }

        if (depth == 0)
        {
            pc = i;
            return;
        }
    }

    // we did not find a match, the program is malformed
    xprintf("Illegal program string.");
    // exit(4);
    exit();
}

void read_char()
{
    buffer[ptr] = (char)getchar();
    pc++;
}

void print_char()
{
    putchar(buffer[ptr]);
    pc++;
}

void decr_byte()
{
    buffer[ptr]--;
    pc++;
}

void incr_byte()
{
    buffer[ptr]++;
    pc++;
}

void move_ptr_bwd()
{
    ptr--;
    pc++;
}

void move_ptr_fwd()
{
    ptr++;
    pc++;
}