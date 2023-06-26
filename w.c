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


t_data	*ft_lstnew_data(t_file *file_name, t_cmd *cmd)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (!new)
		return (NULL);
	new->cmd = cmd;
    new->file = file_name;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_data(t_data **lst, t_data *new)
{
	t_data	*ptr;

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

t_file	*ft_lstnew_file(char *file_name, int type)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (NULL);
    new->filename = file_name;
    new->type = type;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_file(t_file **lst, t_file *new)
{
	t_file	*ptr;

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

t_cmd	*ft_lstnew_cmd(char *cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
    new->cmd = cmd;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

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

t_command	*ft_lstnew(t_token *content)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

t_process	*new_lstnew(void *content)
{
	t_process	*new;

	new = malloc(sizeof(t_process));
	if (!new)
		return (NULL);
	new->content = content;
    new->line = NULL;
    new->cmd = NULL;
	new->next = NULL;
	return (new);
}

void	red_lstadd_back(t_process **lst, t_process *new)
{
	t_process	*ptr;

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
    while (line[i])
    {
        if (line[i] == '<' || line[i] == '>')
        {
            token = create_token(line[i], line[i+1]);
            if (line[i] == line[i + 1])
                i+=2;
            else
                i++;
        }
        else if (line[i] == '|')
            token = create_token(line[i++], '\0');
        else if (line[i] != ' ')
        {
            while ((line[i]) && line[i] != '<' && line[i] != '>' && line[i] != '|' && line[i] != ' ')
            {
                if (!token)
                    token = create_token(line[i], '\0');
                else
                    token->content = add_char(token->content, line[i]);
                i++;
            }
            if (token)
                token->type = TOKEN_STRING;
        }
        else if (line[i] == ' ' || line[i] == '\t')
            i++;
        if (token)
        {
            ft_lstadd_back(lexer, ft_lstnew(token));
            token = NULL;
        }
    }
    if (line[i] == '\0')
    {
        token = create_token(line[i], line[i]);
        ft_lstadd_back(lexer, ft_lstnew(token));
    }
}
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

size_t get_word_end(const char *str)
{
    size_t len = 0;
    while (isalnum(str[len]) || str[len] == '_') len++;
    return len;
}

char *get_env_var(const char *var_name, char **env)
{
    for (int i = 0; env[i] != NULL; i++)
    {
        char *env_var = env[i];
        size_t name_len = strlen(var_name);
        if (strncmp(env_var, var_name, name_len) == 0 && env_var[name_len] == '=') {
            return strdup(env_var + name_len + 1);
        }
    }
    return NULL;
}

char *expand_env_var(const char *token_content, char **env)
{
    if (token_content[1] == '$' || !isalpha(token_content[1]))
        return strdup(token_content);
        
    char *var_name = strdup(token_content + 1);
    char *sep = var_name;
    
    // Find the first non-alphanumeric character
    while (*sep != '\0' && (isalnum(*sep) || *sep == '_')) sep++;
    
    if (*sep != '\0')
    {
        *sep = '\0';
        sep++;
    }
    else 
    {
        sep = NULL;
    }
    
    char *var_value = get_env_var(var_name, env);
    free(var_name);
    
    if (var_value)
    {
        if (sep != NULL) 
        {
            char *expanded = malloc(strlen(var_value) + strlen(sep) + 1);
            strcpy(expanded, var_value);
            strcat(expanded, sep);
            free(var_value);
            return expanded;
        } 
        else 
        {
            return var_value;
        }
    }
    else 
    {
        return strdup(token_content);
    }
}


void expand_env_vars_in_tokens(t_command *lexer, char **env)
{
    while (lexer != NULL) {
        t_token *token = lexer->content;
        if (token->type == TOKEN_STRING && token->content[0] == '$')
        {
            char *expanded_content = expand_env_var(token->content, env);
            free(token->content);
            token->content = expanded_content;
        }
        lexer = lexer->next;
    }
}


int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
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
        expand_env_vars_in_tokens(lexer, env);
        data = get_cmd_file(lexer, data, file, cmd);
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
