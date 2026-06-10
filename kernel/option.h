#ifndef OPTION_H
#define OPTION_H

extern const char* write_options[2];
extern const char* ls_options[2];
extern const char* cat_options[2];
extern const char* rm_options[1];
extern const char* help_options[1];

int option_is_valid(const char* cmd_name, const char* opt);
void execute_option(const char* cmd_name, const char* opt, const char* name, const char* content);

#endif
