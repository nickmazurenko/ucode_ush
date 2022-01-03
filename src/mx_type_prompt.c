#include "../inc/ush.h"

void mx_type_prompt(void) {
    /*static bool prompt_first_time = true;
    if (prompt_first_time) {
        const char *CLEAR_SCREEN_ANSI = " \x1B[1;1H\x1B[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        prompt_first_time = false;
    }*/
    mx_printstr("u$h> ");
}
