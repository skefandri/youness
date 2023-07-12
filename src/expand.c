/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 11:49:05 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/07 11:51:49 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *before_ds(char *str)
{
    char *new;
    int i = 0;
    while (str[i])
    {
        if (str[i] == '$' || str[i] == '\"')
            break;
        i++;
    }
    new  = malloc(i + 1);
    i = 0;
    while (str[i] && check_characters(str[i]) == 0)
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
    char *needle;
    while (str[i] && str[i] != '$' && str[i] != '\"')
        i++;
    needle = malloc(i);
    i = 1;
    while (str[i] && str[i] != '$' && str[i] != '\"')
    {
        needle[j] = str[i];
        i++;
        j++;
    }
    needle[j] = '\0';
    printf("\n\nneedle[[%s]]\n\n", needle);
    return (needle);
}

char *get_val(char *str, char **env)
{
    int i = 0, j = 0;
    char *main;
    char *after;
    char *all;
    t_env   *envs;
    int checker = 0;

    envs = set_envs(env);
    while (str[i] && check_characters(str[i]) == 0)
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
    while (str[i] && check_characters(str[i]) == 0)
    {
        main[i] = str[i];
        i++;
    }
    main[i] = '\0';
    printf("my main [%s]\n\n\n\n\n", main);
    i = 0;
    while(envs)
    {
        if (!strcmp(envs->env, main))
        {
            main = envs->value;
            checker = 1;
            break ;
        }
        envs = envs->next;
    }
    if (checker == 0)
        all = ft_strjoin("\0", after);
    else
        all = ft_strjoin(main, after);
    return (all);
}

char    *let_it(char *str)
{
    char *res;

    int i = 0;
    while (str[i] && str[i] != '\'')
        i++;
    res = malloc(i + 1);
    i = 0;
    while (str[i] && str[i] != '\'')
    {
        res[i] = str[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}
