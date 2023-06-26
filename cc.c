#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    for (int i = 0; env[i] != 0; i++) {
        if (strncmp(env[i], key, len) == 0 && env[i][len] == '=') {
            return &env[i][len + 1];
        }
    }
    return NULL;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *str = "hello$USER.your_name_is$?USER.kkkkk$PATH$USER";
    char *newStr = malloc(strlen(str) * 2 + 1);
    char *var = malloc(strlen(str) + 1);

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '\'' && str[i+1] == '$')
        {
            while(str[i] && str[i] != '\'')
                newStr[j++] = str[i++];
        }
        else if (str[i] == '$')
        {
            i++;
            int k = 0;
            int isExitStatus = (str[i] == '?');
            if(isExitStatus)
                i++;
            while ((isalnum(str[i]) || str[i] == '_') && str[i])
                var[k++] = str[i++];
            var[k] = '\0';

            char *envVal = NULL;
            if(isExitStatus)
            {
                envVal = "0";
                while (*envVal)
                    newStr[j++] = *envVal++;
                envVal = var;
            }
            else 
                envVal = get_env_val(var, env);
            if (envVal != NULL)
            {
                while (*envVal)
                    newStr[j++] = *envVal++;
            }
        }
        else
        {
            newStr[j++] = str[i++];
        }
    }
    newStr[j] = '\0';

    printf("%s\n", newStr);

    free(newStr);
    free(var);
    return 0;
}
