#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    int i = 0;
    while (env[i])
    {
        if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
            return &env[i][len + 1];
        i++;
    }
    return NULL;
}

char *replace_var(const char *str, char **env)
{
    char *newStr = malloc(strlen(str) * 2 + 1);
    char *var = malloc(strlen(str) + 1);

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            // Handle the special cases for $$, $/, and $
            if (str[i] == '$' || str[i] == '/' || str[i] == '\\')
            {
                newStr[j++] = '$';
                newStr[j++] = str[i++];
                continue;
            }
            int isExitStatus = (str[i] == '?');
            if(isExitStatus)
                i++;
            int k = 0;
            while (str[i] != '.' && str[i] != '$' && str[i])
                var[k++] = str[i++];
            var[k] = '\0';
            printf("=======var : %s\n", var);
            char *envVal = NULL;
            if(isExitStatus)
                envVal = "127"; // This should be replaced with actual exit status retrieval mechanism
            else 
                envVal = get_env_val(var, env);
            printf("-------->envVal : %s\n", envVal);
            printf("+++++++newSTR : %s\n", newStr);
            if (envVal != NULL)
            {
                while (*envVal)
                    newStr[j++] = *envVal++;
            }
            printf("=======newSTR : %s\n", newStr);
            printf("-------->var : %s\n", var);
            if(isExitStatus)
            {
                while(*var)
                    newStr[j++] = *var++;
            }
            printf("-------->newSTR : %s\n", newStr);
        }
        else
            newStr[j++] = str[i++];
    }
    newStr[j] = '\0';

    free(var);

    return newStr;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    char *str = "hello$?USER.your_name_is$USER.kkkkk";
    char *newStr = replace_var(str, env);
    printf("%s\n", newStr);

    free(newStr);
    
    return 0;
}
