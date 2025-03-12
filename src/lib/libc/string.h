

#pragma once

#include <fs/xin_structures.h>
#include <lib/libc/file.h>
#include <lib/libc/time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/macros.h>

#include "./string/info/info.h"
#include "./string/ip/ip.h"
#include "./string/manip/manip.h"
#include "./string/parse/parse.h"
#include "./string/search/search.h"
#include "./string/sprintf/sprintf.h"

#define STRING_UNSIGNED 0
#define STRING_SIGNED 1

#define STRCAT_DEST_FIRST true
#define STRCAT_SRC_FIRST false

#ifdef __cplusplus
extern "C" {
#endif

[[nodiscard]] char* getline(XinEntry* File, int line_id);

#ifdef __cplusplus
}
#endif
