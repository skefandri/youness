// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// typedef struct s_env
// {
//     char    *str;
//     char    *env;
//     char    *value;
//     struct s_env *next;
// } t_env;

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	unsigned int	i;
// 	char			*ptr;

// 	if (!s)
// 		return (0);
// 	i = strlen(s);
// 	if (len == 0 || start > i)
// 		return (strdup(""));
// 	if (len > i - start)
// 		len = i - start;
// 	ptr = malloc ((len + 1) * sizeof(char));
// 	if (!ptr)
// 		return (NULL);
// 	i = 0;
// 	while (s[i] && (len > 0))
// 	{
// 		ptr[i] = s[i + start];
// 		i++;
// 		len--;
// 	}
// 	ptr[i] = '\0';
// 	return (ptr);
// }


// void	ft_lstadd_back_env(t_env **lst, t_env *new)
// {
// 	t_env	*ptr;

// 	if (!lst)
// 		return ;
// 	if (!(*lst))
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	ptr = *lst;
// 	while (ptr->next)
// 		ptr = ptr->next;
// 	ptr->next = new;
// }

// char *get_env(char *str)
// {
//     char *new;
//     int i;
//     int j;

//     i = 0;
//     j = 0;
//     while(str[i])
//     {
//         if (str[i] == '=')
//             j = i;
//         i++;
//     }
//     new = ft_substr(str, 0, j);
//     if (!new)
//         strdup("");
//     return (new);
// }

// char *get_value(char *str)
// {
//     char *new;
//     int i;
//     int j;

//     i = 0;
//     j = 0;
//     while(str[i])
//     {
//         if (str[i] == '=')
//             j = i;
//         i++;
//     }
//     new = ft_substr(str, j + 1, i);
//     if (!new)
//         strdup("");
//     return (new);
// }
// t_env	*ft_lstnew_env(char *str)
// {
// 	t_env	*new;

// 	new = malloc(sizeof(t_env));
// 	if (!new)
// 		return (NULL);
// 	new->str = strdup(str);
//     new->env = get_env(str);
//     new->value = get_value(str);
// 	new->next = NULL;
// 	return (new);
// }

// t_env   *set_envs(char **av)
// {
//     int i;
//     t_env   *head = NULL;

//     i = 0;
//     while(av[i])
//         ft_lstadd_back_env(&head, ft_lstnew_env(av[i++]));
//     return (head);
// }

// char    *ft_strjoin(char const *s1, char const *s2)
// {
//     char    *str;
//     int        i;
//     int        j;
//     int        k;

//     i = 0;
//     j = 0;
//     k = 0;
//     if (s1 == NULL || s2 == NULL)
//         return (NULL);
//     str = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
//     if (!str)
//         return (NULL);
//     while (s1[i])
//         str[k++] = s1[i++];
//     while (s2[j])
//         str[k++] = s2[j++];
//     str[k] = '\0';
//     return (str);
// }

// int    check_characters(char line)
// {
//     if (line && line == 95)
//         return (0);
//     if ((line >= 32 && line <= 47) || (line >= 58 && line <= 64)
//         || (line >= 91 && line <= 96) || (line >= 123
//             && line <= 126))
//         return (1);
//     return (0);
// }

// char *before_ds(char *str)
// {
//     char *new;
//     int i = 0;
//     while (str[i])
//     {
//         if (str[i] == '$' || str[i] == '\"')
//             break;
//         i++;
//     }
//     new  = malloc(i + 1);
//     i = 0;
//     while (str[i] && check_characters(str[i]) == 0)
//     {
//         new[i] = str[i];
//         i++;
//     }
//     new[i] = '\0';
//     return (new);
// }

// char *check_what_need(char *str)
// {
//     int i = 1;
//     int j = 0;
//     char *needle;
//     while (str[i] && str[i] != '$' && str[i] != '\"')
//         i++;
//     needle = malloc(i);
//     i = 1;
//     while (str[i] && str[i] != '$' && str[i] != '\"')
//     {
//         needle[j] = str[i];
//         i++;
//         j++;
//     }
//     needle[j] = '\0';
//     printf("\n\nneedle[[%s]]\n\n", needle);
//     return (needle);
// }

// char *get_val(char *str, char **env)
// {
//     int i = 0, j = 0;
//     char *main;
//     char *after;
//     char *all;
//     t_env   *envs;
//     int checker = 0;

//     envs = set_envs(env);
//     while (str[i] && check_characters(str[i]) == 0)
//         i++;
//     if (!str[i])
//     {
//         after = malloc(1);
//         after[0] = '\0';
//     }
//     else
//     {
//         int t = i;
//         while (str[i] && str[i] != '$')
//         {
//             i++;
//             j++;
//         }
//         after = malloc(j + 1);
//         j = 0;
//         while (str[t] && str[t] != '$')
//         {
//             after[j] = str[t];
//             j++;
//             t++;
//         }
//         after[j] = '\0';
//     }
//     printf("my after = [%s]\n", after);
//     main = malloc(i + 1);
//     i = 0;
//     while (str[i] && check_characters(str[i]) == 0)
//     {
//         main[i] = str[i];
//         i++;
//     }
//     main[i] = '\0';
//     printf("my main [%s]\n\n\n\n\n", main);
//     i = 0;
//     while(envs)
//     {
//         if (!strcmp(envs->env, main))
//         {
//             main = envs->value;
//             checker = 1;
//             break ;
//         }
//         envs = envs->next;
//     }
//     if (checker == 0)
//         all = ft_strjoin("\0", after);
//     else
//         all = ft_strjoin(main, after);
//     return (all);
// }

// char    *let_it(char *str)
// {
//     char *res;

//     int i = 0;
//     while (str[i] && str[i] != '\'')
//         i++;
//     res = malloc(i + 1);
//     i = 0;
//     while (str[i] && str[i] != '\'')
//     {
//         res[i] = str[i];
//         i++;
//     }
//     res[i] = '\0';
//     return (res);
// }

// // typedef struct s_expand
// // {
// //     char    *str;
// //     char    *save;
// //     char    *before;
// //     char    *res;
// //     int     check_times;
// //     int     r;
// // }t_expand;

// // void    expand_result(t_expand *expand_value, char **env)
// // {
// //     expand_value->save = NULL;
// //     expand_value->check_times = 0;
// //     expand_value->r = 0;
// //     expand_value->str = "hello$$USER.hjd";

// //     int i = 0;
// //     while(expand_value->str[i])
// //     {
// //         expand_value->save = NULL;
// //         if (expand_value->str[i] == '$')
// //         {
// //             if (expand_value->str[i - 1] && expand_value->str[i - 1] == '\'')
// //                 expand_value->save = let_it(&expand_value->str[i + 1]);
// //             else
// //             {
// //                 if (i != 0 && expand_value->check_times == 0)
// //                 {
// //                     expand_value->before = before_ds(expand_value->str);
// //                     printf("befor = [%s]\n", expand_value->before);
// //                     expand_value->check_times = 1;
// //                 }
// //                 expand_value->save = check_what_need(&expand_value->str[i]);
// //                 printf("what i'm saved  = [%s]\n\n", expand_value->save);
// //                 expand_value->save = get_val(expand_value->save, env);
// //                 printf("what im geted [%s]\n", expand_value->save);
// //             }
// //         }
// //         if (expand_value->r == 0 && expand_value->save)
// //         {
// //             expand_value->res = ft_strjoin("\0", expand_value->save);
// //             expand_value->r = 1;
// //         }
// //         else if (expand_value->r == 1 && expand_value->save)
// //             expand_value->res = ft_strjoin(expand_value->res, expand_value->save);
// //         i++;
// //     }
// //     if (expand_value->before)
// //         expand_value->res = ft_strjoin(expand_value->before, expand_value->res);
// // }
// //specialchar = " \t\n\f\v\r\b"'\`~!@#$%^&*()-=+[]{};:/?.>,<";

// // int main(int ac, char **av, char **env)
// // {
// //     (void)ac;
// //     (void)av;
// //     t_expand *expand_value = malloc(sizeof(t_expand));
// //     expand_result(expand_value, env);
// //     printf("\n%s\n", expand_value->res);
// // }

// int main(int ac, char **av, char **env)
// {
//     (void)ac;
//     (void)av;
//     char *str = "hello$$USER$dhjf";
//     char *save = NULL;
//     char *before;
//     char *res;
//     int i = 0;
//     int check_times = 0;
//     int r = 0;
//     while (str[i])
//     {
//         save = NULL;
//         if (str[i] && str[i] == '$')
//         {
//             if (str[i - 1] && str[i - 1] == '\'')
//                 save = let_it(&str[i + 1]);
//             else
//             {
//                 if (i != 0 && check_times == 0)
//                 {
//                     before = before_ds(str);
//                     printf("before = [%s]\n", before);
//                     check_times = 1;
//                 }
//                 save = check_what_need(&str[i]);
//                 printf("what i'm saved  = [%s]\n\n", save);
//                 save = get_val(save, env);
//                 printf("what im geted [%s]\n", save);
//             }
//         }
//         if (r == 0 && save)
//         {
//             res = ft_strjoin("\0", save);
//             r = 1;
//         }
//         else if (r == 1 && save)
//             res = ft_strjoin(res, save);
//         i++;
//     }
//     if (before)
//         res = ft_strjoin(before, res);
//     printf("\n%s\n", res);
// }
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
		ptr = ptr->next;
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
    t_env   *head = NULL;

    i = 0;
    while(av[i])
        ft_lstadd_back_env(&head, ft_lstnew_env(av[i++]));
    return (head);
}
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

int    check_characters(char *line, int i)
{
    if (line[i] && line[i] == 95)
        return (0);
    if ((line[i] >= 32 && line[i] <= 47) || (line[i] >= 58 && line[i] <= 64)
        || (line[i] >= 91 && line[i] <= 96) || (line[i] >= 123
            && line[i] <= 126))
        return (1);
    return (0);
}

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
    while (str[i] && str[i] != '$' && str[i] != '\"')
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
int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char *str = "hello'$USER'.dhjf";
    char *save = NULL;
    char *before;
    char *res;
    int i = 0;
    int check_times = 0;
    int r = 0;
    while (str[i])
    {
        save = NULL;
        if (str[i] && str[i] == '$')
        {
            if (str[i - 1] && str[i - 1] == '\'')
                save = let_it(&str[i + 1]);
            else
            {
                if (i != 0 && check_times == 0)
                {
                    before = before_ds(str);
                    printf("before = [%s]\n", before);
                    check_times = 1;
                }
                save = check_what_need(&str[i]);
                printf("what i'm saved  = [%s]\n\n", save);
                save = get_val(save, env);
                printf("what im geted [%s]\n", save);
            }
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