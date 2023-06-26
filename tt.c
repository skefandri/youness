#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    int i = 0;
    printf("---------------===================/////////// %s\n", key);
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
    char *newStr = calloc(strlen(str) + 1, 0);
    char *var = calloc(strlen(str) + 1, 0);
    printf("--------------------------------\n");
    printf("==> %s\n", var);
    printf("--------------------------------\n");
    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (str[i] == '$' || str[i] == '/' || str[i] == '\\')
            {
                newStr[j++] = '$';
                newStr[j++] = str[i++];
                printf("--------------------------------\n");
                printf("---->check $ : %s\n", newStr);
                printf("======check str : %s\n", str);
                printf("--------------------------------\n");

                continue;
            }
            printf("======================================================================\n");
            printf("======================================================================\n");
            int isExitStatus = (str[i] == '?');
            printf("---------<><><> int ex : %d\n", isExitStatus);
            if(isExitStatus)
                i++;
            int k = 0;
            while (str[i] != '.' && str[i] != '$' && str[i])
                var[k++] = str[i++];
            var[k] = '\0';
            printf("------>VAR : %s\n", var);
            char *envVal = NULL;
            if(isExitStatus)
                envVal = "0";
            else 
                envVal = get_env_val(var, env);
            printf("------>enVal : %s\n", envVal);
            if (envVal != NULL)
            {
                while (*envVal)
                    newStr[j++] = *envVal++;
            }
            printf("------>------->--------> : %s\n", newStr);
            if (isExitStatus)
            {
                while(*var)
                    newStr[j++] = *var++;
            }
            printf("================== : %s\n", newStr);
         }
        else
            newStr[j++] = str[i++];
        printf("______________________ : %s\n", newStr);
    }
    newStr[j] = '\0';
    printf("============================== : %s\n", newStr);
    free(var);

    return newStr;
}


int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    char *str = "hello$$USER.your_name_is$?USER.kkkkk$USER$PATH$USER$/USER";
    char *newStr = replace_var(str, env);
    printf("%s\n", newStr);

    free(newStr);
    
    return 0;
}
