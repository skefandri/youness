/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:17:37 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/14 22:34:23 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_env
{
	char* key;
	char* value;
	struct s_env* next;
} t_env;

int str_len(const char* str)
{
	int len = 0;
	while (str[len] != '\0')
		len++;
	return len;
}

char    *str_alloc_copy(const char* src)
{
	int len = str_len(src);
	char    *dest = (char*)malloc(len + 1);
	int i = 0;
	while(i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[len] = '\0';
	return dest;
}

t_env   *new_env(char* key, char* value)
{
	t_env* env = (t_env*)malloc(sizeof(t_env));
	env->key = str_alloc_copy(key);
	env->value = str_alloc_copy(value);
	env->next = NULL;
	return env;
}

char    *parse_key_value(char* env)
{
	int i = 0;
	while (env[i] != '=' && env[i] != '\0')
		i++;
	if (env[i] == '=')
		env[i] = '\0';
	return env + i + 1;
}

t_env   *parse_envs(char  **env)
{
	t_env* head = NULL;
	t_env* tail = NULL;

	while (*env)
	{
		char* value = parse_key_value(*env);
		t_env* new_node = new_env(*env, value);
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		env++;
	}
	return head;
}

char    *append_to_str(char* dest, char* src)
{
	int dest_len = str_len(dest);
	int src_len = str_len(src);
	char* new_str = (char*)malloc(dest_len + src_len + 1);
	int i = 0;
	while(i < dest_len)
	{
		new_str[i] = dest[i];
		i++;
	}
	i = 0;
	while(i < src_len)
	{
		new_str[dest_len + i] = src[i];
		i++;
	}
	new_str[dest_len + src_len] = '\0';
	return new_str;
}

bool is_variable_character(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

char    *extract_substring(char* str, int start, int end)
{
	char    *temp = (char*)malloc(end - start + 1);
	int j = start;
	while(j < end)
	{
		temp[j - start] = str[j];
		j++;
	}
	temp[end - start] = '\0';
	return temp;
}


char    *get_env_value(char** env, char* temp, int start, int i)
{
	char* value = NULL;
	int j = 0;
	while(env[j])
	{
		// If the keys match and are followed by '='
		if (strncmp(temp, env[j], i - start) == 0 && env[j][i - start] == '=')
		{
			// New condition: check if temp is longer than the key in env[j]
			// If it is, then the next character in temp is not a valid variable character
			if (temp[i - start] != '\0' && is_variable_character(temp[i - start]))
			{
				j++;
				continue;
			}
			value = env[j] + i - start + 1;
			break;
		}
		j++;
	}
	return value;
}

char    *append_segment(char* result, char* str, int start, int end)
{
	char* temp = extract_substring(str, start, end);
	if (result != NULL) 
	{
		char    *oldResult = result;
		result = append_to_str(oldResult, temp);
		free(oldResult);
	}
	else
		result = temp;
	return result;
}

char    *append_env_value(char* result, char* value)
{
	if (result != NULL) {
		char* oldResult = result;
		result = append_to_str(oldResult, value);
		free(oldResult);
	} 
	else
		result = str_alloc_copy(value);
	return result;
}

int exit_status = 0;
char *expand_result(char *str, char **env)
{
    int i = 0, start = 0;
    char *result = NULL;
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    while (str[i] != '\0')
    {
        if (str[i] == '\'')
        {
            if (!in_double_quotes)
                in_single_quotes = !in_single_quotes;
            i++;
            continue;
        }

        if (str[i] == '\"')
        {
            if (!in_single_quotes)
                in_double_quotes = !in_double_quotes;
            i++;
            continue;
        }

        if (str[i] == '$'  && !in_single_quotes )
        {
            if (str[i+1] == '?')
            {
                char *exit_status_str = ft_itoa(exit_status);
                result = append_segment(result, str, start, i);
                result = append_to_str(result, exit_status_str);
                start = i + 2; 
                i += 2;
                continue;
            }
            else if (str[i+1] == '$')
            {
                
                result = append_segment(result, str, start, i+1);
                start = i+1;
                i+=2;
                continue;
            }
			else if ((str[i+1] >= 48 && str[i+1] <= 57) || str[i + 1] == '@')
            {
                start = i + 2; 
                i += 2;
                result = append_segment(result, str, start, i);
                continue;
            }
			////error I need to fix it
			// else if (i + 2 < (int)strlen(str) && ((str[i + 1] == '\'' && str[i + 2] == '\'') || (str[i + 1] == '"' && str[i + 2] == '"')))
			// {
			// 	start += 1;
			// 	i+=1;
			// }
            int variable_start = i + 1;
            while (str[variable_start] != '\0' && is_variable_character(str[variable_start]))
                variable_start++;
            if (variable_start != i + 1) // Only process $ followed by a variable
            {
                char *temp = extract_substring(str, i + 1, variable_start);
                char *value = get_env_value(env, temp, i + 1, variable_start);
                result = append_segment(result, str, start, i);
                if (value != NULL)
                {
                    result = append_env_value(result, value);
                }
                free(temp);
                start = variable_start;
            }
            else // case when there is only a $ sign without a following variable
            {
                result = append_segment(result, str, start, i + 1);
                start = i + 1;
            }
            i = variable_start;
        }
        else
        {
            i++;
        }
    }
    result = append_segment(result, str, start, i);
    return result;
}
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

char    *remove_quotes(char *line)
{
	int i = 0, j = 0;
	char c;
	while(line[i])
	{
		if (c == 0 && (line[i] == '\'' || line[i] =='\"'))
			c = line[i];
		else if (c == line[i])
			c = 0;
		else
			line[j++] = line[i];
		i++;
	}
	line[j] = '\0';
	return (line);
}

t_data    *get_cmd_file(t_command *lexer, t_data *data, t_file *file, t_cmd *cmd, char **env)
{
	char *expand = NULL;
	t_command *llexer;
	while(lexer)
	{
		if (lexer->content->type != 0 && lexer->content->type != 3 && lexer->content->type != 6)
		{
			expand = expand_result(lexer->next->content->content, env);
			ft_lstadd_back_file(&file, ft_lstnew_file(remove_quotes(expand), lexer->content->type));
			lexer = lexer->next;
		}
		else if (lexer->content->type == 3)
		{
			expand = expand_result(lexer->content->content, env);
			llexer = NULL;
			process_tokens(expand, &llexer);
			while(llexer && llexer->next)
			{
				ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd(remove_quotes(llexer->content->content)));
				llexer = llexer->next;
			}
		}
		else if (lexer->content->type == 0 || lexer->content->type == 6)
		{
			ft_lstadd_back_data(&data, ft_lstnew_data(file, cmd));
			file = NULL;
			cmd = NULL;
		}
		lexer = lexer->next;
	}
	return (data);
}

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
// 	{
// 		ptr = ptr->next;
// 	}
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
//     t_env   *head;

//     i = 0;
//     while(av[i])
//         ft_lstadd_back_env(&head, ft_lstnew_env(av[i++]));
//     return (head);
// }

// void    remove_quotes(char *str)
// {
//     int i = 0;
//     char c;
//     while(str[i])
//     {
//         if (str[i] == '\'' || str[i] == '\"')
//         {
//             c = str[i];
//             i++;
//             while(str[i] != c)
//                 i++;
//             i++;
//         }
//         i++;
//     }
// }

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
		// char *expand = expand_result(line, env);
		// free(lexer->content->content);
		// lexer = NULL;
		// process_tokens(expand, &lexer);
		// remove_quotes(expand);
		t_data *data = NULL;
		t_file *file = NULL;
		t_cmd *cmd = NULL;
		data = get_cmd_file(lexer, data, file, cmd, env);
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
