

#include "elf_loader.hpp"

#warning "ERROR OR struct";

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
    fread(file, &header, SIZE_OF(ElfHeaderAuto));
    fclose(&file);

    return header;
}

std::vector<ElfProgramHeaderAuto> ElfLoader::program_headers_get() const
{
    auto header = header_get();
    uint8_t* program_headers_buffer = (uint8_t*)calloc(header.e_phentsize * header.e_phnum);

    auto file = fopen(m_exepath, "r");
    fread(file, &header, SIZE_OF(ElfHeaderAuto));

    fseek(file, header.e_phoff);
    fread(file, program_headers_buffer, header.e_phentsize * header.e_phnum);

    fclose(&file);

    std::vector<ElfProgramHeaderAuto> program_headers;

    for (int i = 0; i < header.e_phnum; i++) {
        program_headers.push_back(*(ElfProgramHeaderAuto*)(program_headers_buffer + (i * header.e_phentsize)));
    }

    return program_headers;
}

std::vector<ElfSectionHeaderAuto> ElfLoader::section_headers_get(void) const {}

bool ElfLoader::load(uint8_t* address)
{
    m_elf_location = address;
    return true;
    // return fread(m_file, m_elf_location, m_file->size) != 0; // returns true if sth was loaded
}

void ElfLoader::execute(void) const
{
    //check if everything is ok

    auto eheader = header_get();
    auto pheaders = program_headers_get();

    bool is_pie = eheader.e_type == ET_DYN;

    auto file = fopen(m_exepath, "r");

    for (auto& pheader : pheaders)
    {
        if (pheader.p_type == PT_LOAD)
        {
            fseek(file, pheader.p_offset);

            if (is_pie == false) {
                fread(file, (uint8_t*)pheader.p_vaddr, pheader.p_filesz);
                memset((uint8_t*)(pheader.p_paddr + pheader.p_filesz), 0, pheader.p_memsz - pheader.p_filesz); //initialize bss section
                ((void(*)(void))eheader.e_entry)();
            }

            else {
                fread(file, (uint8_t*)s_pie_load_address + pheader.p_vaddr, pheader.p_filesz);
                memset((uint8_t*)(s_pie_load_address + pheader.p_paddr + pheader.p_filesz), 0, pheader.p_memsz - pheader.p_filesz); //initialize bss section
                ((void(*)(void))(eheader.e_entry + s_pie_load_address))();
            }

        }


        // else
        // {
        //     dbg_error(DEBUG_LABEL_KERNEL, "other e_type are not supported");
        // }

    }

}
