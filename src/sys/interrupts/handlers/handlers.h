
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

#ifdef __cplusplus
extern "C" {
#endif

void divide_by_zero_exception_entry(void);
void debug_exception_entry(void);
void nmi_interrupt_exception_entry(void);
void breakpoint_exception_entry(void);
void overflow_exception_entry(void);
void bound_range_exceeded_exception_entry(void);
void invalid_opcode_exception_entry(void);
void device_not_available_exception_entry(void);
void double_fault_exception_entry(void);
void coprocessor_segment_overrun_exception_entry(void);
void invalid_tss_exception_entry(void);
void segment_not_present_exception_entry(void);
void stack_fault_exception_entry(void);
void general_protection_exception_entry(void);
void page_fault_exception_entry(void);
void x86_fpu_floating_point_exception_entry(void);
void aligment_check_exception_entry(void);
void machine_check_exception_entry(void);
void simd_floating_point_exception_entry(void);
void virtualization_exception_entry(void);

#ifdef __cplusplus
}`
#endif