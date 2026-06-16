#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

void cmd_help(const char* input);
void cmd_clear(void);
void cmd_version(void);
void cmd_reboot(void);
void cmd_panic(void);
void cmd_echo(const char* args);
void cmd_ls(const char* input);
void cmd_touch(const char* args);
void cmd_write(const char* input);
void cmd_cat(const char* input);
void cmd_rm(const char* input);
void cmd_exec(const char* input);

#endif