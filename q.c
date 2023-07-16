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

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
    
	len = snprintf(NULL, 0, "%d", n);
	str = (char*)malloc ((sizeof(char) * len) + 1);
	if (!str)
		return (NULL);
	snprintf(str, len + 1, "%d", n);
	return (str);
}

t_env   *new_env(char* key, char* value)
{
    t_env* env = (t_env*)malloc(sizeof(t_env));
    env->key = strdup(key);
    env->value = strdup(value);
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
    char* new_str = (char*)malloc(strlen(dest) + strlen(src) + 1);
    strcpy(new_str, dest);
    strcat(new_str, src);
    return new_str;
}

bool is_variable_character(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

char    *extract_substring(char* str, int start, int end)
{
    char    *temp = (char*)malloc(end - start + 2);
    strncpy(temp, &str[start], end-start);
    temp[end - start] = '\0';
    return temp;
}

char* get_env_value(char** env, char* temp, int start, int i)
{
    char* value = NULL;
    int j = 0;
    while(env[j])
    {
        if (strncmp(temp, env[j], i - start) == 0 && env[j][i - start] == '=')
        {
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
        result = strdup(temp);
    free(temp);
    return result;
}

char    *append_env_value(char* result, char* value)
{
    if (result != NULL) 
    {
        char* oldResult = result;
        result = append_to_str(oldResult, value);
        free(oldResult);
    } 
    else
        result = strdup(value);
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
        if (str[i] == '$'  && !in_single_quotes)
        {
            if (str[i+1] == '?')
            {
                char *exit_status_str = ft_itoa(exit_status);
                result = append_segment(result, str, start, i);
                result = append_to_str(result, exit_status_str);
                free(exit_status_str);
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
                result = append_segment(result, str, start, i);
                start = i + 2;
                i += 2;
                continue;
            }
            int variable_start = i + 1;
            while (str[variable_start] != '\0' && is_variable_character(str[variable_start]))
                variable_start++;
            if (str[i+1] == '\'' || str[i+1] == '\"')
            {
                result = append_segment(result, str, start, i);
                start = i+1;
                i+=2;
                continue;
            }
            if (variable_start != i + 1)
            {
                char *temp = extract_substring(str, i + 1, variable_start);
                char *value = get_env_value(env, temp, i + 1, variable_start);
                result = append_segment(result, str, start, i);
                if (value != NULL)
                    result = append_env_value(result, value);
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

int main(int ac, char* av[], char* env[])
{
    (void)ac;
    (void)av;
    char    *result = expand_result("$\"USER\"$"" aaa$1test", env);
    printf("%s\n", result);
    free(result);
    return 0;
}
