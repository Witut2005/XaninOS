
#include <lib/libcpp/string.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/container/vector.hpp>
#include <fs/loaders/elf/elf_loader.hpp>
#include <programs/tests/test_case.h>

using namespace std;

class TestGlobalConstructor {
public:
    TestGlobalConstructor(uint32_t num) : m_num(num) { dbg_success(DEBUG_LABEL_LIBCPP, "Global constructor successfully executed"); }
    uint32_t m_num;
    static std::vector<ElfLoader::ElfExecutableMemoryInfo> v_global;
};

std::vector<ElfLoader::ElfExecutableMemoryInfo> TestGlobalConstructor::v_global;

class TestStaticConstructor {
public:
    TestStaticConstructor(uint32_t num) : m_num(num) { dbg_success(DEBUG_LABEL_LIBCPP, "Static constructor successfully executed"); }
    uint32_t m_num;
};

class LocalClassWithStaticMemberClass {
public:
    static std::vector<ElfLoader::ElfExecutableMemoryInfo> v_global;
};

std::vector<ElfLoader::ElfExecutableMemoryInfo> LocalClassWithStaticMemberClass::v_global;

constexpr uint32_t global_constructor_initializer = 12345678;
constexpr uint32_t static_constructor_initializer = 12345678;

TestGlobalConstructor uga(global_constructor_initializer);
static TestStaticConstructor nicho(static_constructor_initializer);


// Global and Statis are basically the same in this context
extern "C" __STATUS __cpp_global_constructors_test(void)
{
    TEST_CASE(TestGlobalConstructor)
    {
        EXPECT_EQUAL(uga.m_num, global_constructor_initializer);
    }

    TEST_CASE(TestStaticConstructor)
    {
        EXPECT_EQUAL(nicho.m_num, static_constructor_initializer);
    }

    TEST_CASE(Vector global)
    {
        for (uint32_t i = 0; i < 10; i++) {
            uga.v_global.push_back({ i, i });
        }

        for (uint32_t i = 0; i < 10; i++) {
            ElfLoader::ElfExecutableMemoryInfo tmp = { i,i };
            EXPECT_JUST_TRUE(uga.v_global[i].begin == tmp.begin && uga.v_global[i].size == tmp.size);
            // xprintf("[%d, %d] [%d, %d]\n", uga.v_global[i].begin, uga.v_global[i].size, tmp.begin, tmp.size);
        }
    }

    TEST_CASE(Local class with static vector member)
    {
        LocalClassWithStaticMemberClass local_class;
        for (uint32_t i = 0; i < 10; i++) {
            local_class.v_global.push_back({ i, i });
        }

        for (uint32_t i = 0; i < 10; i++) {
            ElfLoader::ElfExecutableMemoryInfo tmp = { i,i };
            EXPECT_JUST_TRUE(local_class.v_global[i].begin == tmp.begin && local_class.v_global[i].size == tmp.size);
        }
    }

    return XANIN_OK;
}