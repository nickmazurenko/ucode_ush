
#ifndef UTILS_H
#define UTILS_H

int mx_count_substr_new(const char* str, char* sub);
char *mx_strstr_new(const char *haystack, const char *needle);

void mx_print_str_arr(char** arr);

void str_shift_left(char *str, char stop_symbol);
void str_to_upper_case(char* str);
char mx_char_to_upper(char symbol);
int mx_get_substr_index_new(char *str, const char *sub_str);
int mx_atoi(char *);

#endif //UTILS_H
