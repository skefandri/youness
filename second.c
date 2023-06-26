#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    char *str = "hello$$USER.your_name_is$?USER.$USER.kkkkk$PATH$/USER$?USER$\\USER";
    char *newStr = malloc(strlen(str) * 2 + 1);
    char *var = malloc(strlen(str) + 1);

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            if (str[i+1] == '$')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
            }
            else if (str[i+1] == '/' || str[i+1] == '\\')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
                int k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i])
                    var[k++] = str[i++];
                var[k] = '\0';
                char *tmpVar = var;
                while (*tmpVar)
                    newStr[j++] = *tmpVar++;
            }
            else if (str[i+1] == '?')
            {
                newStr[j++] = '0';
                i += 2;
                int k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i])
                    var[k++] = str[i++];
                var[k] = '\0';
                char *tmpVar = var;
                while (*tmpVar)
                    newStr[j++] = *tmpVar++;
                printf("=======%s\n", newStr);
                printf("Var : =======%s\n", var);
            }
            else
            {
                printf("--------->%s\n", newStr);
                i++;
                int k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i])
                    var[k++] = str[i++];
                var[k] = '\0';
                printf("Var : =======%s\n", var);
                char *envVal = get_env_val(var, env);
                printf("envVal : =======%s\n", envVal);
                if (envVal != NULL)
                {
                    while (*envVal)
                        newStr[j++] = *envVal++;
                }
            }
        }
        else
            newStr[j++] = str[i++];
    }
    newStr[j] = '\0';

    printf("%s\n", newStr);

    free(newStr);
    free(var);
    return 0;
}
