
#include <lib/libc/hal.h>
#include <sys/pmmngr/alloc.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/container/vector/vector.hpp>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/lexer.hpp>
#include <sys/devices/com/com.h>
#include <programs/tests/test_case.h>

using namespace std;

extern "C" __STATUS __cpp_xin_test(void)
{

    TEST_CASE(XinFs path parse)
    {
        EXPECT_EQUAL(__nxin_path_parse("/"), "/");
        EXPECT_EQUAL(__nxin_path_parse("/."), "/");
        EXPECT_EQUAL(__nxin_path_parse("/..."), "/...");
        EXPECT_EQUAL(__nxin_path_parse("skibidi/nicho///////////////////ble"), "/skibidi/nicho/ble");
        EXPECT_EQUAL(__nxin_path_parse("/etc/var/variables.conf"), "/etc/var/variables.conf");
    }

    TEST_CASE(XinFs functions test)
    {
        EXPECT_EQUAL(__nxin_parent_folder_path_get("/nicho/fro/ble"), "/nicho/fro");
        EXPECT_EQUAL_FMT("0x%x", __xin_parent_folder_entry_get("/nicho"), __xin_find_entry("/"));
        EXPECT_EQUAL(__nxin_entry_name_extern("/ugabuga/fromini/dupa"), "dupa");
        EXPECT_NOT_EQUAL_FMT("0x%x", __xin_find_entry("/etc/var/variables.conf"), nullptr);
    }

    TEST_CASE(XinFs parent operations)
    {
        __xin_parent_folder_entry_get("/");
    }

    return XANIN_OK;
}