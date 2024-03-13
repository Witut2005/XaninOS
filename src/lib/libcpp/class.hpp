
#pragma once

#define MAKE_OBJECT_NON_COPYABLE(obj) obj(const obj&) = delete; \
                                        obj& operator=(const obj&) = delete;