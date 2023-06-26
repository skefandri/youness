#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char    *ft_strjoin(char const *s1, char const *s2)
{
    char    *str;
    int        i;
    int        j;
    int        k;

    i = 0;
    j = 0;
    k = 0;
    if (s1 == NULL || s2 == NULL)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
    if (!str)
        return (NULL);
    while (s1[i])
        str[k++] = s1[i++];
    while (s2[j])
        str[k++] = s2[j++];
    str[k] = '\0';
    return (str);
}
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

char *replace_variables(const char *str, char **env)
{
    size_t len = strlen(str);
    char *newStr = malloc((len + 1) * sizeof(char));
    if (newStr == NULL)
    {
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
            }
            else if (str[i+1] == '/' || str[i+1] == '\\')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else if (str[i+1] == '?')
            {
                newStr[j++] = '0';
                i += 2;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else
            {
                i++;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
                newStr[j] = '\0';
                char *envVal = get_env_val(newStr, env);
                if (envVal != NULL && inside_double_quote)
                {
                    newStr = ft_strjoin(newStr, envVal);
                    j = strlen(newStr);
                }
                else if (envVal != NULL && !inside_double_quote)
                {
                    char *tmpVar = newStr;
                    newStr = ft_strjoin(newStr, tmpVar);
                    j = strlen(newStr);
                }
            }
        }
        else
            newStr[j++] = str[i++];
    }
    newStr[j] = '\0';

    return newStr;
}

char *get_double_quoted_variable(const char *str, char **env)
{
    char *var = malloc((strlen(str) * 4 + 1) * sizeof(char));
    if (var == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int i = 0, j = 0;
    while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i]) {
        var[j++] = str[i++];
    }
    var[j] = '\0';

    char *envVal = get_env_val(var, env);
    if (envVal != NULL) {
        free(var);
        return strdup(envVal);
    }

    return var;
}

char *get_single_quoted_variable(const char *str)
{
    char *var = malloc((strlen(str) + 1) * sizeof(char));
    if (var == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int i = 0, j = 0;
    while (str[i] != '\'' && str[i]) {
        var[j++] = str[i++];
    }
    var[j] = '\0';

    return var;
}


int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *str = ""$USER"";
    char *newStr = replace_variables(str, env);
    if (newStr == NULL) {
        fprintf(stderr, "Error occurred during variable replacement.\n");
        return 1;
    }

    printf("%s\n", newStr);

    free(newStr);
    return 0;
}
