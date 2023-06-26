#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_env_val(const char *key, char **env);
char *replace_variables(const char *str, char **env);
char *get_double_quoted_variable(const char *str, char **env);
char *get_single_quoted_variable(const char *str);
char *ft_strjoin(const char *s1, const char *s2);

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    for (int i = 0; env[i] != NULL; i++) {
        if (strncmp(key, env[i], len) == 0 && env[i][len] == '=') {
            return &env[i][len + 1];
        }
    }
    return NULL;
}

char *replace_variables(const char *str, char **env)
{
    size_t len = strlen(str);
    char *newStr = malloc((4 * len + 1) * sizeof(char));
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

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
        else if (str[i] == '$' && !inside_single_quote)
        {
            i++;
            if (str[i] == '$')
            {
                newStr[j++] = str[i++];
            }
            else if (str[i] == '/' || str[i] == '\\')
            {
                newStr[j++] = str[i++];
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else if (str[i] == '?')
            {
                newStr[j++] = '0';
                i++;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else
            {
                char key[256];
                int keyLen = 0;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                {
                    key[keyLen++] = str[i++];
                }
                key[keyLen] = '\0';

                char *envVal = get_env_val(key, env);
                if (envVal != NULL && (inside_double_quote || !inside_double_quote))
                {
                    char *tmpStr = strdup(envVal);
                    if (tmpStr == NULL)
                    {
                        fprintf(stderr, "Memory allocation failed.\n");
                        free(newStr);
                        return NULL;
                    }
                    free(newStr);
                    newStr = tmpStr;
                    j = strlen(newStr);
                }
                else if (envVal != NULL && !inside_double_quote)
                {
                    newStr[j] = '\0';
                }
            }
        }
        else
            newStr[j++] = str[i++];
    }
    newStr[j] = '\0';

    return newStr;
}

char *ft_strjoin(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc((len1 + len2 + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *str = "$USER$PWD";
    char *newStr = replace_variables(str, env);
    if (newStr == NULL) {
        fprintf(stderr, "Error occurred during variable replacement.\n");
        return 1;
    }

    printf("%s\n", newStr);

    free(newStr);
    return 0;
}
