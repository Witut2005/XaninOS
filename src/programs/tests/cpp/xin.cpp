
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>

extern std::string __nxin_absolute_path_get(std::string const& name);
extern std::string __nxin_entry_name_extern(std::string const& path);
extern std::string __nxin_path_parse(std::string path);

extern "C" __STATUS __cpp_xin_test(void)
{

    print("test: {}\n", __nxin_absolute_path_get("nicho"));
    print("test2: {}\n", __nxin_entry_name_extern("/ugabuga/fromini/dupa"));
    print("test3: {}\n", __nxin_path_parse("/ugabuga/./fromini/../dupa/././../"));

    return XANIN_OK;
}