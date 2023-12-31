/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 11:08:06 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/13 15:36:36 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_back(t_command **lst, t_command *new)
{
	t_command	*ptr;

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

char *add_char(char *str, char c)
{
    int i = 0;
    char *new;
    if (!str)
    {
        new = malloc(2);
        new[0] = c;
        new[1] = '\0';
    }
    else
    {
        new = malloc(strlen(str) + 2);
        while (str[i])
        {
            new[i] = str[i];
            i++;
        }
        new[i] = c;
        new[i + 1] = '\0';
        free(str);
    }
    return (new);
}

t_token *create_token(char character, char next_char)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    if (character == '<')
    {
        token->content = add_char(NULL, character);
        if (character == next_char)
        {
            token->content = add_char(token->content, character);
            token->type = TOKEN_HERE_DOC;
        }
        else
            token->type = TOKEN_INFILE;
    }
    else if (character == '>')
    {
        token->content = add_char(NULL, character);
        if (character == next_char)
        {
            token->content = add_char(token->content, character);
            token->type = TOKEN_APPEND;
        }
        else
            token->type = TOKEN_OUTFILE;
    }
    else if (character == '|')
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_PIPE;
    }
    else if (character == '\0')
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_EOF;
    }
    else
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_STRING;
    }
    return token;
}


void    process_tokens(char *line, t_command **lexer)
{
    int i = 0;
    t_token *token = NULL;
    char c;
    while (line[i])
    {
        while(line [i] == ' ' || line[i] == '\t')
            i++;
        if (!line[i])
            break;
        token = malloc(sizeof(t_token));
        token->content = NULL;
        while(line[i])
        {
            if (c == 0 && (line[i] == '\'' || line[i] =='\"'))
                c = line[i];
            else if (c == line[i])
                c = 0;
            if (c == 0 && (line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == ' ' || line[i] == '\t'))
                break ;
            token->content = add_char(token->content, line[i]);
            i++;
        }
        if (token->content)
        {
            token->type = TOKEN_STRING;
            ft_lstadd_back(lexer, ft_lstnew(token));
        }
        if (line[i] == '<' || line[i] == '>')
        {
            token = create_token(line[i], line[i+1]);
            if (line[i] == line[i + 1])
                i+=2;
            else
                i++;
            ft_lstadd_back(lexer, ft_lstnew(token));
        }
        else if (line[i] == '|')
        {
            token = create_token(line[i++], '\0');
            ft_lstadd_back(lexer, ft_lstnew(token));
        }
    }
    if (line[i] == '\0')
    {
        token = create_token(line[i], line[i]);
        ft_lstadd_back(lexer, ft_lstnew(token));
    }
}
