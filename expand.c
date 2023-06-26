#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_env_val(const char *key, char **env);
char *replace_variables(char *str, char **env);
char *get_double_quoted_variable( char *str, char **env);
char *get_single_quoted_variable(const char *str);
char    *ft_strjoin(char *s1, char *s2);

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    printf("key : %s\n", key);
    printf("Len : %zu\n", len);
    for (int i = 0; env[i] != 0; i++)
    {
        if (strncmp(key, env[i], len) == 0 && env[i][len] == '=')
        {
            printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-I am here->>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            return &env[i][len + 1];
        }
    }
    return NULL;
}

char *replace_variables(char *str, char **env)
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
            newStr[j++] = str[i++];
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
                while (str[i] != '$' && str[i])
                    newStr[j++] = str[i++];
            }
            else if (str[i+1] == '?')
            {
                newStr[j++] = '0';
                i += 2;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
                //newStr = strdup(&str[i]);
            }
            else
            {
                i++;
                char *tmp;
                int k = 0;
                while (str[i + k] != '.' && str[i + k] != '$' && str[i + k] != '\'' && str[i + k] != '"' && str[i + k])
                    k++;
                tmp = malloc(k + 1);
                k = 0;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    tmp[k++] = str[i++];
                tmp[k] = 0;
                char *envVal = get_env_val(tmp, env);
                free(tmp);
                tmp = NULL;
                if (envVal != NULL && (inside_double_quote || !inside_double_quote))
                {
                    printf("i : %d\n", i);
                    if (envVal == NULL)
                    {
                        fprintf(stderr, "Memory allocation failed.\n");
                        free(newStr);
                        return NULL;
                    }
                    while (*envVal)
                    {
                        newStr[j++] = *envVal;
                        envVal++;
                    }
                    printf("------>>jh>>>%s\n", newStr);
                    j = strlen(newStr);
                }
                else if (envVal != NULL && (!inside_double_quote || inside_double_quote))
                {
                    newStr[j] = '\0';
                }
            }
        }
        else
        {
            newStr[j++] = str[i++];
        }
    }
    newStr[j] = 0;
    printf("NEWSTR----->%s\n", newStr);
    return newStr;
}

// int ft_strlen(char *s)
// {
//     if (!s)
//         return 0;
//     int  i = 0;
//     while(s[i])
//         i++;
//     return i;
// }

char    *ft_strjoin(char *s1, char *s2)
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

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *str = "hello$USER.jhgf";
    char *newStr = replace_variables(str, env);
    if (newStr == NULL) {
        fprintf(stderr, "Error occurred during variable replacement.\n");
        return 1;
    }

    printf("%s\n", newStr);

    free(newStr);
    return 0;
}
