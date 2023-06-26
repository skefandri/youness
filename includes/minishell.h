/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:17:48 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/23 23:08:57 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
    enum
    {
        TOKEN_PIPE,
        TOKEN_INFILE,
        TOKEN_OUTFILE,
        TOKEN_STRING,
        TOKEN_HERE_DOC,
        TOKEN_APPEND,
        TOKEN_EOF
    } type;
    char *content;
} t_token;

typedef struct s_env
{
    char    *str;
    char    *env;
    char    *value;
    struct s_env *next;
} t_env;

typedef struct s_command
{
    t_token *content;
    struct s_command *next;
} t_command;


typedef struct s_cmd
{
    char    *cmd;
    struct s_cmd *next;
} t_cmd;

typedef struct s_file
{
    char    *filename;
    int     type;
    struct s_file *next;
} t_file;

typedef struct DATA_STRUCT
{
    t_cmd               *cmd;
    t_file              *file;
    struct DATA_STRUCT  *next;
} t_data;


typedef struct s_process
{
    char    **cmd;
    char    *content;
    char    *line;
    t_token *token;
    struct s_process *next;
} t_process;


t_command* parse_tokens(char** tokens);
void    process_tokens(char *line, t_command **lexer);
t_token *create_token(char character, char next_char);
char *add_char(char *str, char c);
int check_syntax_errors(t_command *lexer);
int check_quotes(char *input);
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
t_command	*ft_lstnew(t_token *content);
void	red_lstadd_back(t_process **lst, t_process *new);
t_process	*new_lstnew(void *content);
t_data	*ft_lstnew_data(t_file *file_name, t_cmd *cmd);
void	ft_lstadd_back_data(t_data **lst, t_data *new);
t_file	*ft_lstnew_file(char *file_name, int type);
void	ft_lstadd_back_file(t_file **lst, t_file *new);
t_cmd	*ft_lstnew_cmd(char *cmd);
void	ft_lstadd_back(t_command **lst, t_command *new);
void add_argument(t_command* cmd, char* arg);
t_command	*ft_lstnew(t_token *content);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
char    **ft_split(char const *s, char c);
void    *ft_calloc(size_t count, size_t size);
void    ft_bzero(void *s, size_t n);
int     ft_strcmp(const char *s1, const char *s2);
char    *ft_strdup(const char *s1);
int     check_syntax(char **tokens);
void    handle_input_line(char *line, t_command **lexer, t_token **token);
void    handle_input_chars(char *line, int *i, t_command **lexer, t_token **token);
void    handle_lexer(t_command **lexer, t_data **data, t_file **file, t_cmd **cmd);
#endif