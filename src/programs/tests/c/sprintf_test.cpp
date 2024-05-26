#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <programs/tests/test_case.h>
#include <sys/devices/com/com.h>
#include <lib/libcpp/container/vector/vector.hpp>
#include <lib/libcpp/string.h>

using std::string;

extern std::vector <FmtParserInfo> fmt_parse(char* fmt);

extern "C" __STATUS sprintf_test(void)
{
    TEST_CASE(sprintf functions)
    {
        char* buf = (char*)calloc(50);

        EXPECT_EQUAL(string(xsprintf(buf, "%010x", 0xabcd)), string("000000abcd"));
        EXPECT_EQUAL(string(xsprintf(buf, "%010X", 0xabcd)), string("000000ABCD"));
        EXPECT_EQUAL(string(xsprintf(buf, "%u", 123)), string("123"));
        EXPECT_EQUAL(string(xsprintf(buf, "%d", -10)), string("-10"));
        EXPECT_EQUAL(string(xsprintf(buf, "%10c", 'o')), "         o");
        EXPECT_EQUAL(string(xsprintf(buf, "%06s", "ble")), "000ble");
        EXPECT_EQUAL(string(xsprintf(buf, "%6d", -1)), "    -1");
        EXPECT_EQUAL(string(xsprintf(buf, "%y", 0x03272024)), "03-27-2024");
        EXPECT_EQUAL(string(xsprintf(buf, "%t", 0x2130)), "21:30");

        free(buf);
    }

    TEST_CASE(fmt parse function)
    {
        auto v = fmt_parse("%011x%V10c%d");

        SUB_TEST_CASE(first)
        {
            EXPECT_EQUAL(v[0].alignment_char, '0');
            EXPECT_EQUAL(v[0].alignment_count, 11);
            EXPECT_EQUAL(v[0].fmt, 'x');
            xprintf("entry len: %d\n", v[0].entry_lenght);
            // EXPECT_EQUAL(v[0].entry_lenght, );
        }

        SUB_TEST_CASE(second)
        {
            EXPECT_EQUAL(v[1].alignment_char, 'V');
            EXPECT_EQUAL(v[1].alignment_count, 10);
            EXPECT_EQUAL(v[1].fmt, 'c');
            xprintf("entry len: %d\n", v[1].entry_lenght);
        }

        SUB_TEST_CASE(third)
        {
            EXPECT_EQUAL(v[2].alignment_char, ' ');
            EXPECT_EQUAL(v[2].alignment_count, 0);
            EXPECT_EQUAL(v[2].fmt, 'd');
            xprintf("entry len: %d\n", v[2].entry_lenght);
        }
    }

    return XANIN_OK;

}
