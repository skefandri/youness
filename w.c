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

char* get_env_value(char** env, char* temp, int start, int i)
{
    char* value = NULL;
    int j = 0;
    while(env[j])
    {
        if (strncmp(temp, env[j], i - start) == 0 && env[j][i - start] == '=')
        {
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
            {
                in_single_quotes = !in_single_quotes;
            }
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

        if (str[i] == '$' && !in_single_quotes)
        {
            int dollar_count = 0;
            while (str[i + dollar_count] == '$') // Count $ signs
            {
                dollar_count++;
            }
            int variable_start = i + dollar_count;
            while (str[variable_start] != '\0' && is_variable_character(str[variable_start]))
                variable_start++;
            if (variable_start != i + dollar_count)
            {
                char *temp = extract_substring(str, i + dollar_count, variable_start);
                char *value = get_env_value(env, temp, i + dollar_count, variable_start);
                if (value != NULL && dollar_count % 2 == 1) // replace only if the number of dollar signs is odd
                {
                    result = append_segment(result, str, start, i + dollar_count - 1);
                    result = append_env_value(result, value);
                } 
                else // if the dollar count is even, append the dollar signs and variable as is
                {
                    result = append_segment(result, str, start, variable_start);
                }
                free(temp);
                start = variable_start;
            }
            else // case when there are only dollar signs without a following variable
            {
                result = append_segment(result, str, start, i + dollar_count);
                start = i + dollar_count;
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
    char    *result = expand_result("$USER$mnvb$USER", env);
    printf("%s\n", result);
    free(result);
    return 0;
}
