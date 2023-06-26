/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 11:35:49 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/13 11:58:20 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int check_syntax_errors(t_command *lexer)
{
    t_command *current = lexer;
    if (!lexer)
        return 0;
    if (current->content->type == TOKEN_PIPE)
    {
        printf("syntax error near unexpected token %s\n", current->content->content);
        return 1;
    }
    while (current)
    {
        if (current->content->type == TOKEN_INFILE || 
            current->content->type == TOKEN_OUTFILE || current->content->type == TOKEN_HERE_DOC || 
            current->content->type == TOKEN_APPEND)
        {
            if (!current->next || current->next->content->type != TOKEN_STRING)
            {
                printf("syntax error near unexpected token %s\n", current->content->content);
                return 1;
            }
        }
        else if (current->content->type == TOKEN_PIPE)
        {
            if (!current->next->content || !current->next || current->next->content->type == TOKEN_PIPE || current->next->content->type == TOKEN_EOF)
            {
                printf("syntax error near unexpected token %s\n", current->content->content);
                return 1;
            }
        }
        current = current->next;
    }
    return (0);
}

int check_quotes(char *input)
{
    int i;
    char quote;
    
    i = 0;
    while(input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            quote = input[i];
            i++;
            while(input[i] && input[i] != quote)
                i++;
            if (!input[i])
            {
                printf("syntax error near unexpected token quotes \n");
                return 1;
            }
        }
        i++;
    }
    return 0;
}

