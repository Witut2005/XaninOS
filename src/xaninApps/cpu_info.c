#include <cpuid.h>
#include <lib/stdiox.h>
#include <lib/time.h>


enum {
    CPUID_ECX_SSE3         = 1 << 0, 
    CPUID_ECX_PCLMUL       = 1 << 1,
    CPUID_ECX_DTES64       = 1 << 2,
    CPUID_ECX_MONITOR      = 1 << 3,  
    CPUID_ECX_DS_CPL       = 1 << 4,  
    CPUID_ECX_VMX          = 1 << 5,  
    CPUID_ECX_SMX          = 1 << 6,  
    CPUID_ECX_EST          = 1 << 7,  
    CPUID_ECX_TM2          = 1 << 8,  
    CPUID_ECX_SSSE3        = 1 << 9,  
    CPUID_ECX_CID          = 1 << 10,
    CPUID_ECX_SDBG         = 1 << 11,
    CPUID_ECX_FMA          = 1 << 12,
    CPUID_ECX_CX16         = 1 << 13, 
    CPUID_ECX_XTPR         = 1 << 14, 
    CPUID_ECX_PDCM         = 1 << 15, 
    CPUID_ECX_PCID         = 1 << 17, 
    CPUID_ECX_DCA          = 1 << 18, 
    CPUID_ECX_SSE4_1       = 1 << 19, 
    CPUID_ECX_SSE4_2       = 1 << 20, 
    CPUID_ECX_X2APIC       = 1 << 21, 
    CPUID_ECX_MOVBE        = 1 << 22, 
    CPUID_ECX_POPCNT       = 1 << 23, 
    CPUID_ECX_TSC          = 1 << 24, 
    CPUID_ECX_AES          = 1 << 25, 
    CPUID_ECX_XSAVE        = 1 << 26, 
    CPUID_ECX_OSXSAVE      = 1 << 27, 
    CPUID_ECX_AVX          = 1 << 28,
    CPUID_ECX_F16C         = 1 << 29,
    CPUID_ECX_RDRAND       = 1 << 30,
    CPUID_ECX_HYPERVISOR   = 1 << 31,
 
    CPUID_EDX_FPU          = 1 << 0,  
    CPUID_EDX_VME          = 1 << 1,  
    CPUID_EDX_DE           = 1 << 2,  
    CPUID_EDX_PSE          = 1 << 3,  
    CPUID_EDX_TSC          = 1 << 4,  
    CPUID_EDX_MSR          = 1 << 5,  
    CPUID_EDX_PAE          = 1 << 6,  
    CPUID_EDX_MCE          = 1 << 7,  
    CPUID_EDX_CX8          = 1 << 8,  
    CPUID_EDX_APIC         = 1 << 9,  
    CPUID_EDX_SEP          = 1 << 11, 
    CPUID_EDX_MTRR         = 1 << 12, 
    CPUID_EDX_PGE          = 1 << 13, 
    CPUID_EDX_MCA          = 1 << 14, 
    CPUID_EDX_CMOV         = 1 << 15, 
    CPUID_EDX_PAT          = 1 << 16, 
    CPUID_EDX_PSE36        = 1 << 17, 
    CPUID_EDX_PSN          = 1 << 18, 
    CPUID_EDX_CLFLUSH      = 1 << 19, 
    CPUID_EDX_DS           = 1 << 21, 
    CPUID_EDX_ACPI         = 1 << 22, 
    CPUID_EDX_MMX          = 1 << 23, 
    CPUID_EDX_FXSR         = 1 << 24, 
    CPUID_EDX_SSE          = 1 << 25, 
    CPUID_EDX_SSE2         = 1 << 26, 
    CPUID_EDX_SS           = 1 << 27, 
    CPUID_EDX_HTT          = 1 << 28, 
    CPUID_EDX_TM           = 1 << 29, 
    CPUID_EDX_IA64         = 1 << 30,
    CPUID_EDX_PBE          = 1 << 31
};

void cpu_info(void)
{


    unsigned int ecx, edx, ebx;
    char cpu_vendor_string[12];
    char cpu_brand_string[48];

    __get_cpuid(0,0, (uint32_t)cpu_vendor_string, (uint32_t)(&cpu_vendor_string[8]), (uint32_t)(&cpu_vendor_string[4]));
    
    screen_clear();



    __get_cpuid(0x80000002, (uint32_t)cpu_brand_string, (uint32_t)(cpu_brand_string + 4), (uint32_t)(cpu_brand_string + 8), (uint32_t)(cpu_brand_string + 12));
    __get_cpuid(0x80000003, (uint32_t)(cpu_brand_string + 16), (uint32_t)(cpu_brand_string + 20), (uint32_t)(cpu_brand_string + 24), (uint32_t)(cpu_brand_string + 28));
    __get_cpuid(0x80000004, (uint32_t)(cpu_brand_string + 32), (uint32_t)(cpu_brand_string + 36), (uint32_t)(cpu_brand_string + 40), (uint32_t)(cpu_brand_string + 44));



    xprintf("CPU vendor string: ");
    for(int i = 0; i < 12; i++)
        putchar(cpu_vendor_string[i]);
    
    xprintf("\n");

    xprintf("CPU brand: %s\n", cpu_brand_string);

    xprintf("CPU speed: %dMHz\n\n", cpu_mhz);

    __get_cpuid(0,1, &ebx, &ecx, &edx);

    xprintf("EBX flags features: 0x%x\n", ebx);
    xprintf("ECX flags features: 0x%x\n", ecx);
    xprintf("EDX flags features: 0x%x\n\n", edx);

    char* cpu_state[2] = {"NOT PRESENT", "PRESENT"};
    bool is_present;
    uint8_t cpu_state_color[2] = {red, green};


    is_present = (edx & CPUID_EDX_IA64) >> 30;
    xprintf("64-bit:               %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]); 
     
    is_present = (edx & CPUID_EDX_HTT) >> 28;
    xprintf("Hyper-Threading:      %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_MMX) >> 23;
    xprintf("MMX:                  %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_MCA) >> 14;
    xprintf("Machine Check Arch:   %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_FPU) >> 0;
    xprintf("Onboard FPU:          %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_MSR) >> 5;
    xprintf("MSRs:                 %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_APIC) >> 9;
    xprintf("APIC:                 %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);


    is_present = (ecx & CPUID_ECX_X2APIC) >> 21;
    xprintf("x2APIC:               %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (edx & CPUID_EDX_ACPI) >> 22;
    xprintf("ACPI:                 %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);


    is_present = (ecx & CPUID_ECX_AVX) >> 28;
    xprintf("AVX:                  %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (ecx & CPUID_ECX_RDRAND) >> 30;
    xprintf("RDRAND Instruction:   %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);


    is_present = (edx & CPUID_EDX_SSE2) >> 26;
    xprintf("SSE2:                 %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (ecx & CPUID_ECX_SSE3) >> 0;
    xprintf("SSE3:                 %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);

    is_present = (ecx & CPUID_ECX_SSE4_1) >> 19;
    xprintf("SSE4_1:               %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);
 
    is_present = (ecx & CPUID_ECX_SSE4_2) >> 20;
    xprintf("SSE4_2:               %z%s\n", set_output_color(cpu_state_color[is_present],white), cpu_state[is_present]);


    while(getscan() != ENTER);

}