
#pragma once

#define KERNEL_MODULE

#include <stdint.h>
#include <lib/elf/elf.h>
#include <lib/libcpp/class.hpp>
#include <sys/devices/com/com.h>
#include <lib/libcpp/error.hpp>
#include <lib/libcpp/container/array.hpp>
#include <lib/libcpp/container/vector.hpp>

#undef KERNEL_MODULE

struct ElfMainHeaders
{
    ElfHeaderAuto elf_header;
    std::vector<ElfProgramHeaderAuto> program_headers;
    std::vector<ElfSectionHeaderAuto> section_headers;
};

class ElfLoader {

public:

    enum class Archtecture {
        bit32 = 1,
        bit64 = 2
    };

    enum class Type {
        Relocatable = 1,
        Executable = 2,
        Shared = 3,
        Core = 4
    };

#ifdef __x86_64__
    static constexpr Archtecture s_xanin_native_arch = Archtecture::bit64;
#else
    static constexpr Archtecture s_xanin_native_arch = Archtecture::bit32;
#endif

    static constexpr uint32_t s_pie_load_address = 0x800000;

    static constexpr char s_valid_magic[4] = { 0x7f, 0x45, 0x4c, 0x46 };

    constexpr bool is_valid_arch(Archtecture arch) const {
        return  s_xanin_native_arch == arch;
    }


    ElfLoader(const char* path);
    ~ElfLoader(void);

    [[nodiscard]] bool magic_check(const ElfHeaderAuto&);

    [[nodiscard]] ErrorOr<ElfHeaderAuto> header_get(void) const;
    [[nodiscard]] std::vector<ElfProgramHeaderAuto> program_headers_get(void) const;
    [[nodiscard]] std::vector<ElfSectionHeaderAuto> section_headers_get(void) const;
    [[nodiscard]] ErrorOr<ElfMainHeaders> main_headers_get(void) const;

    bool is_loadable_segment(const ElfProgramHeaderAuto& pheader) const;
    bool load_segment(const ElfProgramHeaderAuto& pheader) const;
    bool execute(void) const;

private:

    static constexpr enum ELF_PROGRAM_HEADER_TYPE s_ignored_segments[] = { PT_PHDR };

    bool m_loaded{ false };
    uint8_t* m_elf_location{ nullptr };
    char* m_exepath;

};