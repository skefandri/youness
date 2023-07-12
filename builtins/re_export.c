/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:06:48 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/11 21:58:40 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "./includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct s_env
{
    char    *str;
    char    *env;
    char    *value;
    struct s_env *next;
} t_env;

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*ptr;

	if (!s)
		return (0);
	i = strlen(s);
	if (len == 0 || start > i)
		return (strdup(""));
	if (len > i - start)
		len = i - start;
	ptr = malloc ((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] && (len > 0))
	{
		ptr[i] = s[i + start];
		i++;
		len--;
	}
	ptr[i] = '\0';
	return (ptr);
}


void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

char *get_env(char *str)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
    {
        if (str[i] == '=')
            j = i;
        i++;
    }
    new = ft_substr(str, 0, j);
    if (!new)
        strdup("");
    return (new);
}

char *get_value(char *str)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
    {
        if (str[i] == '=')
            j = i;
        i++;
    }
    new = ft_substr(str, j + 1, i);
    if (!new)
        strdup("");
    return (new);
}
t_env	*ft_lstnew_env(char *str)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->str = strdup(str);
    new->env = get_env(str);
    new->value = get_value(str);
	new->next = NULL;
	return (new);
}
t_env   *set_envs(char **av)
{
    int i;
    t_env   *head;

    i = 0;
    while(av[i])
        ft_lstadd_back_env(&head, ft_lstnew_env(av[i++]));
    return (head);
}

void    declare_envs(t_env *env)
{
    t_env   *tmp;

    tmp = env;
    while (tmp)
    {
        if (tmp->value)
        {
            printf("declare -x %s=\"", tmp->env);
            printf("%s\"\n", tmp->value);
        }
        else
        {
            printf("declare -x %s\n", tmp->env);
        }
        tmp = tmp->next;
    }
}


void    declare_envs(void)
{
    t_env    *tmp;

    tmp = g_global.envs;
    while(tmp)
    {
        if (tmp->value)
        {
            printf("declare -x %s=", tmp->env);
            printf("\"%s\"\n", tmp->value);
        }
        else
            printf("declare -x %s\n", tmp->str);
        tmp = tmp->next;
    }
    return ;
}

static int    ft_isalnums(int an)
{
    if ((an >= 48 && an <= 57) || (an >= 65 && an <= 90)
        || (an >= 95 && an <= 122))
        return (1);
    else
        return (0);
}

int is_valid_start(char c)
{
    return ((c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z') || c == '_');
}

int is_valid_identifier(char *str)
{
    if (!ft_strchr(str, '='))
        return (is_valid_start(str[0]));
    char *key = ft_substr(str, 0, ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
    int i = 0;
    if (!key[i])
        return (0);
    if (!(key[i] >= 'a' && key[i] <= 'z') && !(key[i] >= 'A' && key[i] <= 'Z') && key[i] != '_')
        return (0);
    while(key[i])
    {
        if (!ft_isalnums(key[i]) && key[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void    add_to_env(char *str)
{
    t_env    *node;
    t_env *tmp;

    tmp = g_global.envs;
    node = ft_lstnew(str);
    while (tmp)
    {
        if (!ft_strcmp(tmp->env, node->env))
        {
            free(tmp->value);
            tmp->value = node->value;
            free(node);
            return ;
        }
        tmp = tmp->next;
    }
    ft_lstadd_back(&g_global.envs, node);
}

void    re_export(char **args)
{
    int    i;

    i = 0;
    if (!args[i])
    {
        declare_envs();
        return ;
    }
    while (args[i])
    {
        if (is_valid_identifier(args[i]))
            add_to_env(args[i]);
        else
            printf("minishell: export: `%s': not a valid identifier\n", args[i]);
        i++;
    }
}

