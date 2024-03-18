
#pragma once

#define XANIN_EXTERNAL_TASKS_CODE_SEGMENT 0x30
#define XANIN_EXTERNAL_TASKS_DATA_SEGMENT 0x38
#define XANIN_EXTERNAL_TASKS_STACK_SEGMENT 0x40

void task_load(uint16_t cs, uint16_t ds, uint32_t where_to_jump) __attribute__((fastcall));