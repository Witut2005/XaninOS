#pragma once

#ifdef __cplusplus
extern "C"{
#endif

extern int screenshot(void);
extern int xagame_test(void);
extern int tetris(void);

int edit(char* file_name);
void scan(void);

#ifdef __cplusplus
}
#endif

extern int last_command_exit_status;
extern int argc;
extern char* argv[5];

