
/* macros */

#pragma once 

#define PIC1_COMMAND_REG 0x20
#define PIC1_STATUS_REG 0x20

#define PIC1_DATA_REG 0x21    
#define PIC1_MASK_REG 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_STATUS_REG 0xA0

#define PIC2_DATA 0xA1    
#define PIC_MASK_REG 0xA1


#define VGA_TEXT_MEMORY 0xb8000

#include <programs/register_dump.h>

extern void divide_by_zero_exception_entry(void);
extern void debug_exception_entry(void);
extern void nmi_interrupt_exception_entry(void);
extern void breakpoint_exception_entry(void);
extern void overflow_exception_entry(void);
extern void bound_range_exceeded_exception_entry(void);
extern void invalid_opcode_exception_entry(void);
extern void device_not_available_exception_entry(void);
extern void double_fault_exception_entry(void);
extern void coprocessor_segment_overrun_exception_entry(void);
extern void invalid_tss_exception_entry(void);
extern void segment_not_present_exception_entry(void);
extern void stack_fault_exception_entry(void);
extern void general_protection_exception_entry(void);
extern void page_fault_exception_entry(void);
extern void x86_fpu_floating_point_exception_entry(void);
extern void aligment_check_exception_entry(void);
extern void machine_check_exception_entry(void);
extern void simd_floating_point_exception_entry(void);
extern void virtualization_exception_entry(void);