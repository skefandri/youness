/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:23:28 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/12 09:23:31 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>

char    *remove_spaces(char *lexer)
{
    char    *str;
    int        i;

    i = 0;
    while (lexer[i] && lexer[i] != ' ')
        i++;
    str = malloc(i + 1);
    if (!str)
        return (0);
    i = 0;
    while (lexer[i] && lexer[i] != ' ')
    {
        str[i] = lexer[i];
        i++;
    }
    str [i] = '\0';
    return (str);
}

void    here_documents(t_list *lexer /*your lexer*/)
{
    t_vals        *first; //to get val of the node
    char        *str; //fot delemetre
    char        *arr; //compare the value of the node with the delemetre

    first = (t_vals *) lexer->content;
    while (first->token != 2 /*token of end of file*/)
    {
        if (first->token == 1 /*token of herdoc*/)
        {
            lexer = lexer->next; //go on on your linked list
            first = (t_vals *) lexer->content; //save the value every time you looping until get the eof
            if (first->token == 0 /*token of the string*/)
            {
                arr = remove_spaces(first->val); //prpare the value to know the right delemetre
                while (1)
                {
                    str = readline("heredoc> "); //use readline to read from the startndart
                    if (!str) //chekcer for control D it mean you have to break
                        break ;
                    if (!ft_strncmp(str, arr, ft_strlen(str)) //compare the delemetr that you have in arr with what you get from readline return
                        && ft_strlen(str) == ft_strlen(arr)) // you need to break the loop if the comapre is 0 because you find the delemetre
                        break; // don't forget free the str
                }
            }
        }
        lexer = lexer->next;
        first = (t_vals *) lexer->content;
    }
}