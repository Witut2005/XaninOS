
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/container/array/array.hpp>

using namespace std;
using namespace std::literals;

extern "C" __STATUS __cpp_array_test(void)
{
   std::array<int, 1> x; 

    return XANIN_OK;
}