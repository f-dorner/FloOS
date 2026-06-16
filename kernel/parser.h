#ifndef PARSER_H
#define PARSER_H

void parser_extract_options_no_args(const char* input, char* cmd_name, char options[][32], int opt_count);
void parser_extract_options_one_arg(const char* input, char* arg1, char options[][32], int opt_count);
int parser_starts_with(const char* str, const char* prefix);

#endif