
#include <stdint.h>
#include <sys/panic_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

    void kernel_panic(uint32_t error_code);

#ifdef __cplusplus
}
#endif