
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <fs/xin.h>

bool elf_check_magic(uint8_t* file);
bool elf_check_arch(uint8_t* file);
void elf_load(XinEntry* file);
