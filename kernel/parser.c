#include "parser.h"
#include "string.h"

void parser_extract_args(const char* input, char* arg1, char* arg2, char* opt)
{
    //first word = name, then content, optional --option at the end
    for (int i = 0; input[i] != '\0'; i++)
    {
        arg1[i] = input[i];

        if (input[i] == ' ') {
            arg1[i] = '\0';
            int k = 0;
            for (int j = (i + 1); input[j] != '\0'; j++)
            {
                if (input[j] == ' ' && input[j+1] == '-' && input[j+2] == '-') {
                    arg2[k] = '\0';
                    int z = 0;
                    for (int l = (j + 3); input[l] != '\0'; l++)
                    {
                        opt[z] = input[l];
                        z++;
                    }
                    opt[z] = '\0';
                    return;
                } 

                if (input[j] == '-' && input[j+1] == '-') {
                    arg2[k] = '\0';
                    int z = 0;
                    for (int l = (j + 2); input[l] != '\0'; l++)
                    {
                        opt[z] = input[l];
                        z++;
                    }
                    opt[z] = '\0';
                    return;
                }

                arg2[k] = input[j];
                k++;
            }
            arg2[k] = '\0';
            return;
        }
    }
}

void parser_extract_option_no_args(const char* input, char* cmd_name, char* opt)
{
    for (int i = 0; input[i] != '\0'; i++) {
        cmd_name[i] = input[i];

        if (input[i] == ' ' && input[i+1] == '-' && input[i+2] == '-') {
            cmd_name[i] = '\0';
            int k = 0;
            for (int j = (i + 3); input[j] != '\0'; j++) {
                opt[k] = input[j];
                k++;
            }
            opt[k] = '\0';
            return;
        }
    }
}

void parser_extract_options_no_args(const char* input, char* cmd_name, char options[][32], int opt_count)
{
    int option_count = 0;

    //for commands like ls --all --long
    for (int i = 0; input[i] != '\0'; i++) {
        cmd_name[i] = input[i];

        if (input[i] == ' ' && input[i+1] == '-' && input[i+2] == '-') {
            cmd_name[i] = '\0';
            if (option_count >= opt_count) {
                return;
            }

            int k = 0;
            int j = i + 3;
            //only copying the name after --, so --long becomes long
            for (; input[j] != ' ' && input[j] != '\0' && k < 31; j++) {
                options[option_count][k] = input[j];
                k++;
            }

            options[option_count][k] = '\0';
            option_count++;

            if (input[j] == '\0') {
                return;
            }

            i = j;
        }
    }
}

void parser_extract_options_one_arg(const char* input, char* arg1, char options[][32], int opt_count) {
    int option_count = 0;

    //for commands like cat h --size --hex or rm h --force
    for (int i = 0; input[i] != '\0'; i++) {

        if (input[i] == ' ') {
            int k = 0;
            for (int j = (i + 1); input[j] != '\0'; j++) {
                if (input[j] == ' ' && input[j+1] == '-' && input[j+2] == '-') {
                    arg1[k] = '\0';
                    if (option_count >= opt_count) {
                        return;
                    }

                    int z = 0;
                    int u = j + 3;
                    //copying option until the next space
                    for (; input[u] != ' ' && input[u] != '\0' && z < 31; u++) {
                        options[option_count][z] = input[u];
                        z++;
                    }

                    j = u;
                    options[option_count][z] = '\0';
                    option_count++;

                    if (input[j] == '\0') {
                        return;
                    }

                    continue;
                }

                arg1[k] = input[j];
                k++;
            }
            arg1[k] = '\0';
            return;
        }
    }
}

int parser_starts_with(const char* str, const char* prefix)
{
    int i = 0;

    while(prefix[i] != '\0')
    {
        if (str[i] != prefix[i])
        {
            return 0;
        }

        i++;
    }

    return 1;
}
