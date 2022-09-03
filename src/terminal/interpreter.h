#pragma once


#include <app_config.h>

#include <libc/stdiox.h>

#ifdef HELP_APP
#include <xaninApps/help.c>
#endif

//#include <xaninApps/ls.c>
//#include <xaninApps/type.c>
//#include <xaninApps/note.c>
//#include <xaninApps/hexview.c>
//#include <xaninApps/execute.c>
//#include <xaninApps/cd.c>
//#include <xaninApps/pwd.c>
//#include <xaninApps/md.c>
//#include <xaninApps/rd.c>
//#include <xaninApps/touch.c>

#ifdef SHUTDOWN_APP
#include <xaninApps/shutdown.c>
#endif

#ifdef REBOOT_APP
#include <xaninApps/reboot.c>
#endif

#ifdef DEVICE_INFO_APP
#include <xaninApps/device_info.c>
#endif

#ifdef LOAD_APP
#include <xaninApps/load.c>
#endif

#ifdef LOADCH_APP
#include <xaninApps/loadch.c>
#endif

#ifdef EPILEPSY_APP
#include <xaninApps/epilepsy.c>
#endif

#ifdef KEYBOARD_TEST_APP
#include <xaninApps/keyboard_test.c>
#endif

#ifdef LOGO_APP
#include <xaninApps/logo.c>
#endif

#ifdef REGISTER_DUMP_APP
#include <xaninApps/register_dump.c>
#endif

#ifdef CPU_INFO_APP
#include <xaninApps/cpu_info.c>
#endif

#ifdef CALC_APP
#include <xaninApps/calc.c>
#endif

#ifdef PONG_APP 
#include <xaninApps/pong/pong.c>
#endif

#ifdef TIMER_TEST_APP
#include <xaninApps/timer_test.c>
#endif

#ifdef ZSK_APP
#include <xaninApps/zsk.c>
#endif

#ifdef EXECUTE_ADDR_APP
#include <xaninApps/execute_addr.c>
#endif

#ifdef DISK_LOAD_APP
#include <xaninApps/disk_load.c>
#endif

#ifdef DISK_WRITE_APP
#include <xaninApps/disk_write.c>
#endif

#ifdef INSTALL_APP
#include <xaninApps/install.c>
#endif

#ifdef STACK_FRAME_TEST_APP
#include <xaninApps/stack_frame_test.c>
#endif

#ifdef RUN_APP
#include <xaninApps/run.c>
#endif

#ifdef RUN16_APP
#include <xaninApps/run16.c>
#endif

#ifdef LIST_FILES_APP
#include <xin_fs/list_files.c>
#endif

#ifdef HEXEDITOR_APP
#include <xaninApps/hexeditor/hexeditor.c>
#endif

#ifdef MOUSE_APP
#include <xaninApps/mouse.c>
#endif

#include <xin_fs/xin.h>

#ifdef NOTE_APP
#include <xin_fs/xin_note.c>
#endif

#ifdef PAINT_APP
#include <xin_fs/xin_paint.c>
#endif

#ifdef CAT_APP
#include <xin_fs/cat.c>
#endif

#ifdef XIN_MOVE_APP
#include <xin_fs/move.c>
#endif

#ifdef XIN_COPY_APP
#include <xin_fs/copy.c>
#endif

#ifdef XIN_LINK_APP 
#include <xin_fs/link.c>
#endif

#ifdef XIN_INFO_APP
#include <xaninApps/file_system/xin_info.c>
#endif

#ifdef ELF_LOADER_APP
#include <elf/elf_loader.c>
#endif

#ifdef CPP_TEST_APP
extern void cpp_test(void);
#endif



extern void screenshot(void);
extern void xagame_test(void);
extern int tetris(void);

#include <xaninApps/nic_info.c>
#include <gyn_cl/gyn.c>
#include <xin_fs/xin_xpaint.c>
#include <xin_fs/xin_apps.c>
#include <xaninApps/xgl_test.c>

#include <xaninApps/arp_table_print.c>
#include <xaninApps/ping.c>
#include <xaninApps/ip_test.c>
#include <xaninApps/udp_test.c>
//extern void xagame_test(void);
//extern void assembler(char* file_name);

void scan(void);

