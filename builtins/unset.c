t_env    *ft_lstlastt(t_env *lst)
{
    t_env    *head;

    if (lst == NULL)
        return (NULL);
    while (lst)
    {
        head = lst;
        lst = lst -> next;
    }
    return (head);
}

void ft_env_free(t_env *node)
{
    free(node->str);
    free(node->env);
    free(node->value);
    free(node);
}

void ft_remove(char *str)
{
    // add valid identifier check
    t_env *del;
    t_env *tmp = g_global.envs;        

    if (!is_valid_identifier(str))
    {
        printf("minishell: unset: `%s': not a valid identifier\n", str);
        return ;
    }
    if (!ft_strcmp(tmp->env, str))
    {
        g_global.envs = tmp->next;
        ft_env_free(tmp);
        return ;
    }
    while (tmp->next)
    {
        if (!ft_strcmp(tmp->next->env, str))
        {
            del = tmp->next;
            tmp->next = del->next;
            ft_env_free(del);
            return ;
        }
        tmp = tmp->next;
    }
}


void    re_unset(char **args)
{
    int    i;

    i = 0;
    if (!args[i])
        return ;
    while (args[i])
    {
        ft_remove(args[i]);
        i++;
    }
}