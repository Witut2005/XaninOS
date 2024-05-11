
#define KERNEL_MODULE

#include "elf_loader.hpp"
#include <lib/libcpp/memory.hpp>
#include <lib/libcpp/error.hpp>
#include <lib/libcpp/algorithm.h>

#undef KERNEL_MODULE 
// #define ELF_LOADER_DEBUG_PIE_ADDRESSES

ElfLoader::ElfLoader(const char* path)
{
    m_exepath = strdup(path);
}

ElfLoader::~ElfLoader(void)
{
    free(m_exepath);
}

bool ElfLoader::magic_check(const ElfHeaderAuto& header)
{
    return bmemcmp(&header, s_valid_magic, EI_MAG_SIZE);
}

ErrorOr<ElfHeaderAuto> ElfLoader::header_get(void) const
{
    ElfHeaderAuto header;

    auto file = fopen(m_exepath, "r");

    if (file == nullptr) { return {}; }

    fread(file, &header, sizeof(ElfHeaderAuto));
    fclose(&file);

    return header;
}

std::vector<ElfProgramHeaderAuto> ElfLoader::program_headers_get() const
{
    TRY(auto header, header_get(), {});

    uint8_t* program_headers_buffer = (uint8_t*)calloc(header.e_phentsize * header.e_phnum);

    auto file = fopen(m_exepath, "r");
    if (file == nullptr) { return {}; }

    fread(file, &header, sizeof(ElfHeaderAuto));

    __xin_fseek(file, header.e_phoff);
    fread(file, program_headers_buffer, header.e_phentsize * header.e_phnum);

    fclose(&file);

    std::vector<ElfProgramHeaderAuto> program_headers;

    for (int i = 0; i < header.e_phnum; i++) {
        program_headers.push_back(*(ElfProgramHeaderAuto*)(program_headers_buffer + (i * header.e_phentsize)));
    }

    return program_headers;
}

std::vector<ElfSectionHeaderAuto> ElfLoader::section_headers_get(void) const {
    TRY(auto header, header_get(), {});

    std::UniquePtr<ElfSectionHeaderAuto> data((ElfSectionHeaderAuto*)calloc(header.e_shentsize * header.e_shnum));

    auto file = fopen(m_exepath, "r");
    __xin_fseek(file, header.e_shoff);
    fread(file, data.get(), header.e_shentsize * header.e_shnum);

    std::vector<ElfSectionHeaderAuto> section_headers;

    for (int i = 0; i < header.e_shnum; i++) {
        section_headers.push_back(*(data.get() + i));
    }

    fclose(&file);
    return section_headers;
}

ErrorOr<ElfMainHeaders> ElfLoader::main_headers_get(void) const
{
    TRY(auto header, header_get(), ErrorOr<ElfMainHeaders>());
    ElfMainHeaders main_headers = { header, program_headers_get(), section_headers_get() };
    return main_headers;
}

bool ElfLoader::is_loadable_segment(const ElfProgramHeaderAuto& pheader) const
{
    for (int i = 0; i < ARRAY_LENGTH(s_ignored_segments); i++) {
        if (pheader.p_type == s_ignored_segments[i]) { return false; }
    }
    return true;
}

bool ElfLoader::load_segment(const ElfProgramHeaderAuto& pheader) const
{
    auto bss_section_size_get = [](const ElfProgramHeaderAuto pheader) -> size_t { return pheader.p_filesz > pheader.p_memsz ? 0 : pheader.p_memsz - pheader.p_filesz;};

    TRY(auto header, header_get(), {});

    bool is_pie = header.e_type == ET_DYN;
    auto file = fopen(m_exepath, "r");

    if (is_loadable_segment(pheader) == false) { return false; }

    __xin_fseek(file, pheader.p_offset);
    uint32_t pie_loadable_memory = find_loadable_memory_location(file->size);

    switch (pheader.p_type)
    {
    case PT_LOAD:
    {

        if (is_pie == false)
        {
            // dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading LOAD segment (no pie)");
            fread(file, (uint8_t*)pheader.p_vaddr, pheader.p_filesz);
            memset((uint8_t*)(pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        }

        else
        {
            // dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading LOAD segment (with pie)");
            fread(file, (uint8_t*)pie_loadable_memory + pheader.p_vaddr, pheader.p_filesz);
            memset((uint8_t*)(pie_loadable_memory + pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        }
        break;
    }

    case PT_DYNAMIC:
    {
        // dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading DYNAMIC segment");
        fread(file, (uint8_t*)(pie_loadable_memory + pheader.p_vaddr), pheader.p_filesz);
        memset((uint8_t*)(pie_loadable_memory + pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        break;
    }

    }

    fclose(&file);
    return true;
}

bool ElfLoader::execute(void)
{
    TRY(auto header, header_get(), {});
    auto pheaders = program_headers_get();

    bool is_pie = header.e_type == ET_DYN;

    auto file = fopen(m_exepath, "r");
    if (file == nullptr) { return false; }

    for (auto& pheader : pheaders) {
        load_segment(pheader);
    }

    uint32_t entry_point = header.e_entry + (is_pie ? find_loadable_memory_location(file->size) : 0x0);

    s_pie_load_addresses_used.push_back({ entry_point, file->size });

#ifdef ELF_LOADER_DEBUG_PIE_ADDRESSES
    for (const auto a : s_pie_load_addresses_used) {
        dbg_warning(DEBUG_LABEL_ELF_LOADER, xsprintf(std::UniquePtr<char>((char*)calloc(50)).get(), "[0x%x, 0x%x]", a.begin, a.begin + a.size));
    }
#endif

    ((void(*)(void))(entry_point))(); // jump to entry point
    return true;
}

uint32_t ElfLoader::find_loadable_memory_location(uint32_t size) const
{
    uint32_t begin = s_pie_load_addresses_begin;

    for (auto a : s_pie_load_addresses_used)
    {
        if (std::have_intersection<uint32_t>({ begin, begin + size }, { a.begin, a.begin + a.size })) {
            begin = a.begin + a.size;
        }
        else {
            break;
        }
    }
    return begin;
}

std::vector<ElfLoader::ElfExecutableMemoryInfo> ElfLoader::s_pie_load_addresses_used; //TODO problems with global constructor