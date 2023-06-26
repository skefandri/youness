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
    new  = malloc(i + 1);
    i = 0;
    while (str[i] && str[i] != '$')
    {
        new[i] = str[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

char *check_what_need(char *str)
{
    int i = 1;
    int j = 0;
    //printf("%c\n", str[0]);
    char *needle;
    while (str[i] && str[i] != '$')
        i++;
    needle = malloc(i);
    i = 1;
    while (str[i] && str[i] != '$')
    {
        needle[j] = str[i];
        i++;
        j++;
    }
    needle[j] = '\0';
    return (needle);
}

char *get_val(char *str, char **env)
{
    int i = 0, j = 0;
    char *main;
    char *after;
    char *all;

    while (str[i] && str[i] != '.')
        i++;
    if (!str[i])
    {
        after = malloc(1);
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
        j = 0;
        while (str[t] && str[t] != '$')
        {
            after[j] = str[t];
            j++;
            t++;
        }
        after[j] = '\0';
    }
    printf("my after = [%s]\n", after);
    main = malloc(i + 1);
    i = 0;
    while (str[i] && str[i] != '.')
    {
        main[i] = str[i];
        i++;
    }
    main[i] = '\0';
    printf("my main [%s]\n\n\n\n\n", main);
    i = 0;
    while(env[i])
    {
        if (!strncmp(env[i], main, strlen(main)))
        {
            j = 0;
            while (env[i][j] && env[i][j] != '=')
                j++;
            j += 1;
            main = &env[i][j];
        }
        i++;
    }
    all = ft_strjoin(main, after);
    return (all);
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char *str = "hello$$USER.your_name_is$USER.kkkkk$PATH";
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
            if (str[i - 1] && str[i - 1] == '\'')
                i++;
            if (i != 0 && check_times == 0)
            {
                before = before_ds(str);
                printf("befor = [%s]\n", before);
                check_times = 1;
            }
            save = check_what_need(&str[i]);
            printf("what i'm saved  = [%s]\n\n", save);
            save = get_val(save, env);
            printf("what im geted [%s]\n", save);
        }
        if (r == 0 && save)
        {
            res = ft_strjoin("\0", save);
            r = 1;
        }
        else if (r == 1 && save)
            res = ft_strjoin(res, save);
        i++;
    }
    if (before)
        res = ft_strjoin(before, res);
    printf("\n%s\n", res);
}