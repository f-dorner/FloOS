#include "option.h"
#include "string.h"

const char* write_options[1] = {"clear"};
const char* ls_options[2] = {"all", "long"};
const char* cat_options[2] = {"size", "hex"};
const char* rm_options[1] = {"force"};
const char* help_options[1] = {"options"};

int option_is_valid(const char* cmd_name, const char* opt)
{
    //0 = valid, 1 = not found
    if (strcmp(cmd_name, "write") == 0) {
        for (int i = 0; i < 1; i++) {
            if (strcmp(opt, write_options[i]) == 0) {
                return 0;
            }
        }
    } else if (strcmp(cmd_name, "ls") == 0) {
        for (int i = 0; i < 2; i++)  {
            if (strcmp(opt, ls_options[i]) == 0) {
                return 0;
            }
        }
    } else if (strcmp(cmd_name, "cat") == 0) {
        for (int i = 0; i < 2; i++) {
            if (strcmp(opt, cat_options[i]) == 0) {
                return 0;
            }
        }
    } else if (strcmp(cmd_name, "rm") == 0) {
        for (int i = 0; i < 1; i++) {
            if (strcmp(opt, rm_options[i]) == 0) {
                return 0;
            }
        }
    } else if (strcmp(cmd_name, "help") == 0) {
        for (int i = 0; i < 1; i++) {
            if (strcmp(opt, help_options[i]) == 0) {
                return 0;
            }
        }
    }

    return 1;
}
