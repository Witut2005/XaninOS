
#include <lib/libc/hal.h>
#include <lib/libc/stdlibx.h>
#include <programs/tests/test_case.h>
#include <sys/pmmngr/alloc.h>

__STATUS __c_alloc_test(void)
{
    interrupt_disable();

    TEST_CASE(allocation)
    {
        uint8_t* ptr = (uint8_t*)ucalloc(100);
        free(ptr);
        uint8_t* ptr2 = (uint8_t*)ucalloc(100);
        EXPECT_EQUAL_FMT("0x%x", ptr, ptr2);
        ptr = ucalloc(100);
        EXPECT_NOT_EQUAL_FMT("0x%x", ptr, ptr2);

        free(ptr);
        free(ptr2);
    }

    TEST_CASE(heap place check)
    {
        uint8_t* kptr = (uint8_t*)kcalloc(100);
        uint8_t* uptr = (uint8_t*)ucalloc(100);

        EXPECT_ABOVE_FMT("0x%x", uptr, kptr);

        free(kptr);
        free(uptr);
    }

    interrupt_enable();

    return XANIN_OK;
}
