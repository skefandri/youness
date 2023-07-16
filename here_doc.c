/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:23:28 by ysabr             #+#    #+#             */
/*   Updated: 2023/07/16 13:41:48 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

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

void    here_document(t_command *lexer /*your lexer*/)
{
    char        *str; //fot delemetre
    char        *arr; //compare the value of the node with the delemetre
    
    while (lexer)
    {
        if (strstr(lexer->content->content, "<<"))
        {
            lexer = lexer->next;
            arr = remove_spaces(lexer->content->content);
            printf("\n[%s]\n", arr);
            while (1)
            {
                str = readline("heredoc>> ");
                printf("[%s]\n", str);
                if (!str)
                    break;
                //exit(0);
                if (!strncmp(str, arr, strlen(str)) && strlen(str) == strlen(arr))
                {
                    printf("%d\n", 9999999);
                    break;
                }
            }
        }
        lexer = lexer->next;
    }
    // {
    //     printf("%s\n", lexer->content->content);
    //     lexer = lexer->next;
    // }

    
    // first = (t_data *) lexer->content;
    // while (first->file->type != 6 /*token of end of file*/)
    // {
    //     if (first->file->type == 4 /*token of herdoc*/)
    //     {
    //         lexer = lexer->next; //go on on your linked list
    //         first = (t_data *)lexer->content->content; //save the value every time you looping until get the eof
    //         if (first->token->content->type == 3 /*token of the string*/)
    //         {
    //             arr = remove_spaces(first->file->filename); //prpare the value to know the right delemetre
    //             while (1)
    //             {
    //                 str = readline("heredoc> "); //use readline to read from the startndart
    //                 if (!str) //chekcer for control D it mean you have to break
    //                     break ;
    //                 if (!ft_strncmp(str, arr, ft_strlen(str)) //compare the delemetr that you have in arr with what you get from readline return
    //                     && ft_strlen(str) == ft_strlen(arr)) // you need to break the loop if the comapre is 0 because you find the delemetre
    //                     break; // don't forget free the str
    //             }
    //         }
    //     }
    //     lexer = lexer->next;
    //     first = (t_vals *) lexer->content;
    // }
}