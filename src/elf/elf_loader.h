
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <xin_fs/xin.h>

bool elf_check_magic(xin_entry* file);
bool elf_check_arch(xin_entry* file);
void elf_load(xin_entry* file);