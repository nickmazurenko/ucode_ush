#include "../inc/ush.h"

char *mx_strrep(char *str, char *substr, char *replace) {
    char *output = malloc(PATH_MAX);
    mx_memset(output, 0, PATH_MAX);
    int i = 0, j = 0, flag = 0, start = 0;
    // check whether the substring to be replaced is present 
    while (str[i] != '\0')
    {
            if (str[i] == substr[j]) 
            {
                    if (!flag)
                            start = i;
                    j++;
                    if (substr[j] == '\0')
                            break;
                    flag = 1;
            } 
            else 
            {
                    flag = start = j = 0;
            }
            i++;
    }

    if (substr[j] == '\0' && flag)
    {
            for (i = 0; i < start; i++)
                    output[i] = str[i];
            // replace substring with another string 
            for (j = 0; j < mx_strlen(replace); j++) 
            {
                    output[i] = replace[j];
                    i++;
            }
            // copy remaining portion of the input string "str" 
            for (j = start + mx_strlen(substr); j < mx_strlen(str); j++)
            {
                    output[i] = str[j];
                    i++;
            }
            // print the final string 
            output[i] = '\0';
            return output;
    } else {
            free(output);
            return NULL;
    }
}
