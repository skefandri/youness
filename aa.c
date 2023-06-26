#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ft_strjoin(char const *s1, char const *s2) 
{
    char *str;
    int i = 0;
    int j = 0;
    int k = 0;

    if (s1 == NULL || s2 == NULL)
        return NULL;

    str = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));

    if (!str)
        return NULL;

    while (s1[i])
        str[k++] = s1[i++];
    while (s2[j])
        str[k++] = s2[j++];
    str[k] = '\0';

    return str;
}

char *before_ds(char *str) 
{
    char *new;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '$')
            break;
        i++;
    }

    new = malloc(i + 1);
    if (!new)
        return NULL;

    i = 0;
    while (str[i] && str[i] != '$')
    {
        new[i] = str[i];
        i++;
    }
    new[i] = '\0';
    return new;
}

char *check_what_need(char *str) 
{
    char *needle;
    int i = 1;
    int j = 0;

    while (str[i] && str[i] != '$')
        i++;

    needle = malloc(i);
    if (!needle)
        return NULL;

    i = 1;
    while (str[i] && str[i] != '$')
    {
        needle[j] = str[i];
        i++;
        j++;
    }
    needle[j] = '\0';

    return needle;
}

char *get_val(char *str, char **env) 
{
    int i = 0, j = 0;
    char *main_var;
    char *after;
    char *all;

    while (str[i] && str[i] != '.')
        i++;
    if (!str[i])
    {
        after = malloc(1);
        if (!after)
            return NULL;
        after[0] = '\0';
    }
    else
    {
        int t = i;
        while (str[i] && str[i] != '$')
        {
            i++;
            j++;
        }
        after = malloc(j + 1);
        if (!after)
            return NULL;
        j = 0;
        while (str[t] && str[t] != '$')
        {
            after[j] = str[t];
            j++;
            t++;
        }
        after[j] = '\0';
    }

    main_var = malloc(i + 1);
    if (!main_var)
    {
        free(after);
        return NULL;
    }
    i = 0;
    while (str[i] && str[i] != '.')
    {
        main_var[i] = str[i];
        i++;
    }
    main_var[i] = '\0';

    i = 0;
    while(env[i])
    {
        if (!strncmp(env[i], main_var, strlen(main_var)) && env[i][strlen(main_var)] == '=')
        {
            free(main_var);
            j = strlen(main_var) + 1;
            main_var = &env[i][j];
            break;
        }
        i++;
    }

    all = ft_strjoin(main_var, after);
    free(after);
    return all;
}

int main(int ac, char **av, char **env) 
{
    (void)ac;
    (void)av;
    char *str = "hello$USER.your_name_is$USER.kkkkk$PATH$USER$USER";
    char *save = NULL;
    char *before;
    char *res;
    int i = 0;
    int check_times = 0;
    int r = 0;

    while (str[i])
    {
        save = NULL;
        if (str[i] == '$')
        {
            if (i != 0 && check_times == 0)
            {
                before = before_ds(str);
                check_times = 1;
            }
            save = check_what_need(&str[i]);
            save = get_val(save, env);
            i += strlen(save);
        }
        if (r == 0 && save)
        {
            res = ft_strjoin("\0", save);
            r = 1;
        }
        else if (r == 1 && save)
        {
            char *temp = res;
            res = ft_strjoin(res, save);
            free(temp);
        }
        i++;
    }
    if (before)
    {
        char *temp = res;
        res = ft_strjoin(before, res);
        free(temp);
        free(before);
    }
    printf("\n%s\n", res);
    free(res);
    return 0;
}
