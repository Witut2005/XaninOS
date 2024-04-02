
#pragma once

#include <stdint.h>
#include <lib/elf/elf.h>
#include <lib/libcpp/class.hpp>
#include <sys/devices/com/com.h>
#include <lib/libcpp/container/vector.hpp>

class ElfLoader {
    // MAKE_OBJECT_NON_COPYABLE(ElfLoader)

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

    [[nodiscard]] ElfHeaderAuto header_get(void) const;
    [[nodiscard]] std::vector<ElfProgramHeaderAuto> program_headers_get(void) const;
    [[nodiscard]] std::vector<ElfSectionHeaderAuto> section_headers_get(void) const;

    bool load(uint8_t* address);
    void execute(void) const;

private:

    bool m_loaded{ false };
    uint8_t* m_elf_location{ nullptr };
    char* m_exepath;

};