
#define KERNEL_MODULE

#include "elf_loader.hpp"
#include <lib/libcpp/memory.hpp>

#warning "TODO ERROR OR struct";

#undef KERNEL_MODULE

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

ElfHeaderAuto ElfLoader::header_get(void) const
{
    ElfHeaderAuto header;

    auto file = fopen(m_exepath, "r");

    if (file == nullptr) { return {}; }

    fread(file, &header, SIZE_OF(ElfHeaderAuto));
    fclose(&file);

    return header;
}

std::vector<ElfProgramHeaderAuto> ElfLoader::program_headers_get() const
{
    auto header = header_get();
    uint8_t* program_headers_buffer = (uint8_t*)calloc(header.e_phentsize * header.e_phnum);

    auto file = fopen(m_exepath, "r");
    if (file == nullptr) { return {}; }

    fread(file, &header, SIZE_OF(ElfHeaderAuto));

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
    auto pheader = header_get();

    std::UniquePtr<ElfSectionHeaderAuto> data((ElfSectionHeaderAuto*)calloc(pheader.e_shentsize * pheader.e_shnum));

    auto file = fopen(m_exepath, "r");
    __xin_fseek(file, pheader.e_shoff);
    fread(file, data.get(), pheader.e_shentsize * pheader.e_shnum);

    std::vector<ElfSectionHeaderAuto> section_headers;

    for (int i = 0; i < pheader.e_shnum; i++)
    {
        section_headers.push_back(*(data.get() + i));
    }

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

    auto eheader = header_get();
    bool is_pie = eheader.e_type == ET_DYN;

    auto file = fopen(m_exepath, "r");

    if (is_loadable_segment(pheader) == false) { return false; }

    __xin_fseek(file, pheader.p_offset);

    switch (pheader.p_type)
    {
    case PT_LOAD:
    {

        if (is_pie == false)
        {
            dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading LOAD segment (no pie)");
            fread(file, (uint8_t*)pheader.p_vaddr, pheader.p_filesz);
            memset((uint8_t*)(pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        }

        else
        {
            dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading LOAD segment (with pie)");
            fread(file, (uint8_t*)s_pie_load_address + pheader.p_vaddr, pheader.p_filesz);
            memset((uint8_t*)(s_pie_load_address + pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        }
        break;
    }

    case PT_DYNAMIC:
    {
        dbg_info(DEBUG_LABEL_ELF_LOADER, "Loading DYNAMIC segment");
        fread(file, (uint8_t*)(s_pie_load_address + pheader.p_vaddr), pheader.p_filesz);
        memset((uint8_t*)(s_pie_load_address + pheader.p_paddr + pheader.p_filesz), 0, bss_section_size_get(pheader)); //initialize bss section
        break;
    }

    }

    fclose(&file);

    return true;
}

bool ElfLoader::execute(void) const
{
    //check if everything is ok

    auto eheader = header_get();
    auto pheaders = program_headers_get();

    bool is_pie = eheader.e_type == ET_DYN;

    auto file = fopen(m_exepath, "r");
    if (file == nullptr) { return false; }

    for (auto& pheader : pheaders) {
        load_segment(pheader);
    }

    ((void(*)(void))(eheader.e_entry + (is_pie ? s_pie_load_address : 0x0)))();
    return true;
}
