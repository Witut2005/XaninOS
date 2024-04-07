
#include <lib/libcpp/error.hpp>
#include <lib/libcpp/memory.hpp>
#include <sys/devices/com/com.h>
#include <fs/loaders/elf/elf_loader.hpp>

extern "C" {

    __STATUS elf_loader_test(char* path)
    {
        ElfLoader loader(path);

        TRY(auto headers, loader.main_headers_get(), []() {xprintf("invalid data\n"); return XANIN_ERROR; }());

        xprintf("program headers size: %d\n", headers.program_headers.size());
        xprintf("section headers size: %d\n", headers.section_headers.size());

        return XANIN_OK;
    }
}