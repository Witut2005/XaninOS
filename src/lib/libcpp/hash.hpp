
#pragma once

#include <stdint.h>
#include <lib/libcpp/string.h>

uint32_t jhash(std::string str);
uint32_t jhash(int i);
uint32_t jhash(uint32_t i);