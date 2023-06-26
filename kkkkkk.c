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

    char *str = "hello$$USER.your_name_is$?USER.kkkkk$USER$PATH$USER$/USER";
    char *newStr = malloc(5 * strlen(str) + 1);
    char *var = malloc(strlen(str) * 4 + 1);

    int inside_double_quote = 0;
    int inside_single_quote = 0;

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '"')
        {
            inside_double_quote = !inside_double_quote;
            i++;
        }
        else if (str[i] == '\'')
        {
            inside_single_quote = !inside_single_quote;
            i++;
        }
        else if (str[i] == '$' && inside_single_quote)
        {
            newStr[j++] = str[i++];
        }
        else if (str[i] == '$' && !inside_single_quote)
        {
            if (str[i+1] == '$')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
                //printf("------->%s\n", newStr);
            }
            else if (str[i+1] == '/' || str[i+1] == '\\')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
                int k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
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
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    var[k++] = str[i++];
                var[k] = '\0';
                char *tmpVar = var;
                while (*tmpVar)
                    newStr[j++] = *tmpVar++;
            }
            else
            {
                i++;
                int k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    var[k++] = str[i++];
                var[k] = '\0';
                char *envVal = get_env_val(var, env);
                if (envVal != NULL && inside_double_quote)
                {
                    while (*envVal)
                        newStr[j++] = *envVal++;
                }
                else if (envVal != NULL && !inside_double_quote)
                {
                    char *tmpVar = var;
                    while (*tmpVar)
                        newStr[j++] = *tmpVar++;
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
