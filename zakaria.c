#include "./minishell.h"
#include <ctype.h>

t_command	*ft_lstnew(void *content)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

t_process    *split_pipe(char *line)
{
    t_process   *split_pipe = NULL;
    int           i;
    char quote;
    int j = 0;

    i = 0;
    if (!line)
        return NULL;
    while(line[i])
    {
        if (line[i] == '\'' || line[i] == '"')
        {
            quote = line[i];
            i++;
            while(line[i] && line[i] != quote)
                i++;
            i++;
        }
        else if (line[i] == '|')
        {
            red_lstadd_back(&split_pipe, new_lstnew(ft_substr(line, j, i - j)));
            j = ++i;
        }
        else
            i++;
    }
    red_lstadd_back(&split_pipe, new_lstnew(ft_substr(line, j, i - j)));
    return (split_pipe);
}

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
    }
    return (new);
}

int    main()
{
    while(1)
    {
        char *line = readline("minishell> ");
        if (!line)
            break;
        if (line)
            add_history(line);
        char *str;
        int i = 0;
        t_command *lexer = NULL;
        while (1)
        {
            if (!line[i])
                break;
            while(line[i])
            {
                str = NULL;
                if (line[i] == '<')
                {
                    if (line[i + 1] == '<')
                    {
                        str = malloc(3);
                        str[0] = '<';
                        str[1] = '<';
                        str[2] = '\0';
                        i+=2;
                    }
                    else
                    {
                        str = malloc(2);
                        str[0] = '<';
                        str[1] = '\0';
                        i++;
                    }
                }
                else if (line[i] == '>')
                {
                    if (line[i + 1] == '>')
                    {
                        str = malloc(3);
                        str[0] = '>';
                        str[1] = '>';
                        str[2] = '\0';
                        i+=2;
                    }
                    else
                    {
                        str = malloc(2);
                        str[0] = '>';
                        str[1] = '\0';
                        i++;
                    }
                }
                else if (line[i] == '|')
                {
                    str = malloc(2);
                    str[0] = '|';
                    str[1] = '\0';
                    i++;
                }
                else if (line[i] == ' ')
                {
                    i++;
                    continue;
                }
                else
                {
                    while ((line[i]))
                    {
                        if (line[i] == '<' || line[i] == '>' || line[i] == '|')
                            break;
                        str = add_char(str, line[i]);
                        i++;
                    }
                }
                ft_lstadd_back(&lexer, ft_lstnew(str));
            }
        }
        while (lexer)
        {
            printf("%s\n", lexer->content);
            lexer = lexer->next;
        }
    }
}