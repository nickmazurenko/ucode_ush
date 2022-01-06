#include "ush.h"

char *mx_replace_substr_new(char *str, char *substr, char *replace)
{
        char *output = malloc(PATH_MAX);
        mx_memset(output, 0, PATH_MAX);
        int i = 0, j = 0, flag = 0, start = 0;
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
                for (j = 0; j < mx_strlen(replace); j++)
                {
                        output[i] = replace[j];
                        i++;
                }
                for (j = start + mx_strlen(substr); j < mx_strlen(str); j++)
                {
                        output[i] = str[j];
                        i++;
                }
                output[i] = '\0';
                return output;
        }
        else
        {
                free(output);
                return NULL;
        }
}
