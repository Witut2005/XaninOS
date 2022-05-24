
#pragma once

enum APIC_REGISTERS
{

    APIC_BASE                               = 0xFEE00000,
    LOCAL_APIC_ID                           = 0xFEE00020,
    LOCAL_APIC_VERSION                      = 0xFEE00030,
    APIC_TASK_PRIORITY_REGISTER             = 0xFEE00080,
    APIC_ARBITRATION_PRIORITY_REGISTER      = 0xFEE00090, 
    APIC_PROCESSOR_PRIORITY_REGISTER        = 0xFEE000A0,
    APIC_EOI_REGISTER                       = 0xFEE000B0,
    APIC_REMOTE_READ_REGISTER               = 0xFEE000C0,
    APIC_LOCAL_DESTINATION_REGISTER         = 0xFEE000D0,
    APIC_DESTINATION_FORMAT_REGISTER        = 0xFEE000E0,
    APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER = 0xFEE000F0, 
    
    APIC_ISR_31_0                           = 0xFEE00100,
    APIC_ISR_63_32                          = 0xFEE00110,
    APIC_ISR_95_64                          = 0xFEE00120,
    APIC_ISR_127_96                         = 0xFEE00130,
    APIC_ISR_159_128                        = 0xFEE00140,
    APIC_ISR_191_160                        = 0xFEE00150,
    APIC_ISR_223_192                        = 0xFEE00160,
    APIC_ISR_255_224                        = 0xFEE00170,

    APIC_TMR_31_0                           = 0xFEE00180,
    APIC_TMR_63_32                          = 0xFEE00190,
    APIC_TMR_95_64                          = 0xFEE001A0,
    APIC_TMR_127_96                         = 0xFEE001B0,
    APIC_TMR_159_128                        = 0xFEE001C0,
    APIC_TMR_191_160                        = 0xFEE001D0,
    APIC_TMR_223_192                        = 0xFEE001E0,
    APIC_TMR_255_224                        = 0xFEE001F0,

    APIC_IRR_31_0                           = 0xFEE00200,
    APIC_IRR_63_32                          = 0xFEE00210,
    APIC_IRR_95_64                          = 0xFEE00220,
    APIC_IRR_127_96                         = 0xFEE00230,
    APIC_IRR_159_129                        = 0xFEE00240,
    APIC_IRR_191_160                        = 0xFEE00250,
    APIC_IRR_223_192                        = 0xFEE00260,
    APIC_IRR_255_224                        = 0xFEE00270,

    APIC_ERROR_STATUS_REGISTER              = 0xFEE00280,
    APIC_LVT_CMCI_REGISTER                  = 0xFEE002F0,
    APIC_ICR_31_0                           = 0xFEE00300,
    APIC_ICR_63_32                          = 0xFEE00310,
    APIC_LVT_TIMER_REGISTER                 = 0xFEE00320,
    APIC_LVT_REGISTER                       = 0xFEE00320,
    APIC_LVT_THERMAL_SENSOR_REGISTER        = 0xFEE00330,
    APIC_LVT_PMC_REGISTER                   = 0xFEE00340, // Performance monitoring counters
    APIC_LVT_LINT0_REGISTER                 = 0xFEE00350,
    APIC_LVT_LINT1_REGISTER                 = 0xFEE00360, 
    APIC_LVT_ERROR_REGISTER                 = 0xFEE00370,
    APIC_INITIAL_COUNT_REGISTER             = 0xFEE00380,
    APIC_CURRENT_COUNT_REGISTER             = 0xFEE00390,

    APIC_DIVIDE_CONFIGURATION_REGISTER      = 0xFEE003E0

};


enum LVT_OPTIONS
{
    LVT_UNMASK = (1 << 16),
    APIC_UNMASK = (1 << 17)
};