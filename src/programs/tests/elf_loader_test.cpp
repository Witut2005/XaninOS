
#include <fs/loaders/elf/elf_loader.hpp>
#include <sys/devices/com/com.h>

extern "C" {

    __STATUS elf_loader_test(char* path)
    {
        ElfLoader loader(path);
        loader.execute();
        return XANIN_OK;

        // auto header = loader.header_get();
        // auto program_headers = loader.program_headers_get();

        // char* sprintfbuf = (char*)calloc(100);

        // dbg_info(DEBUG_LABEL_PROCESS, xsprintf(sprintfbuf, "%02x %02x %02x %02x", header.ei_mag[0], header.ei_mag[1], header.ei_mag[2], header.ei_mag[3]));

        // dbg_info(DEBUG_LABEL_PROCESS, xsprintf(sprintfbuf, "vector size: %d", program_headers.size()));

        // for (auto a : program_headers) {
        //     dbg_info(DEBUG_LABEL_PROCESS, xsprintf(sprintfbuf, "%d", a.p_type));
        // }

        // free(sprintfbuf);
    }
}