
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>
#include <programs/tests/test_case.h>

using std::string;

extern std::string __nxin_absolute_path_get(std::string const& name);
extern std::string __nxin_entry_name_extern(std::string const& path);
extern std::string __nxin_path_parse(std::string path);
extern string __nxin_parent_folder_get(string path);
extern XinEntry* __nxin_parent_folder_entry_get(const char* path);

extern "C" __STATUS __cpp_xin_test(void)
{

    TEST_CASE(XinFs path parse)
    {
        EXPECT_EQUAL(__nxin_path_parse("/"), "/");
        EXPECT_EQUAL(__nxin_path_parse("/etc/var/variables.conf"), "/etc/var/variables.conf");
    }

    TEST_CASE(XinFs functions test)
    {
        EXPECT_EQUAL(__nxin_parent_folder_get("/nicho/fro/ble"), "/nicho/fro");
        EXPECT_EQUAL_FMT("0x%x", __nxin_parent_folder_entry_get("/nicho"), __xin_find_entry("/"));
        EXPECT_EQUAL(__nxin_entry_name_extern("/ugabuga/fromini/dupa"), "dupa");
        EXPECT_NOT_EQUAL_FMT("0x%x", __xin_find_entry("/etc/var/variables.conf"), nullptr);
    }



    return XANIN_OK;
}