/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:17:37 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/24 21:32:35 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int read_line(char **line)
{
    *line = readline("minishell> ");
    if (!(*line))
        return 1;
    add_history(*line);
    return 0;
}

void display_data(t_data *data)
{
    while (data)
    {
        while (data->cmd && data->cmd->cmd)
        {
            printf("cmd: %s\n", data->cmd->cmd);
            data->cmd = data->cmd->next;
        }
        while (data->file && data->file->filename)
        {
            printf("file: %s | type: %d\n", data->file->filename, data->file->type);
            data->file = data->file->next;
        }
        printf("<-------------------- next node ---------------->\n");
        data = data->next;
    }
}

t_data    *get_cmd_file(t_command *lexer, t_data *data, t_file *file, t_cmd *cmd)
{
    while(lexer)
    {
        if (lexer->content->type != 0 && lexer->content->type != 3 && lexer->content->type != 6)
        {
            ft_lstadd_back_file(&file, ft_lstnew_file(lexer->next->content->content, lexer->content->type));
            lexer = lexer->next;
        }
        else if (lexer->content->type == 3)
             ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd(lexer->content->content));
        if (lexer->content->type == 0 || lexer->content->type == 6)
        {
            ft_lstadd_back_data(&data, ft_lstnew_data(file, cmd));
            file = NULL;
            cmd = NULL;
        }
        lexer = lexer->next;
    }
    return (data);
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

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    (void)env;
    //t_env *head = set_envs(env);
    
    while(1)
    {
        char *line;
        if (read_line(&line))
            break;
        t_command *lexer = NULL;
        process_tokens(line, &lexer);
        if (check_syntax_errors(lexer) || check_quotes(line))
            continue;
        t_data *data = NULL;
        t_file *file = NULL;
        t_cmd *cmd = NULL;
        data = get_cmd_file(lexer, data, file, cmd);
        // if (strcmp(data->cmd->cmd, "export"))
        //     re_export(env);
        t_command *lst = NULL;
        lexer = lst;
        while(lexer)
        {
            t_command *next = lexer->next;
            free(lexer->content->content);
            free(lexer->content);
            free(lexer);
            lexer = next;
        }
        display_data(data);
    }
    return 0;
}
