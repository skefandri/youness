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

t_env   *new_env(char* key, char* value)
{
    t_env* env = (t_env*)malloc(sizeof(t_env));

    int key_len = str_len(key);
    env->key = (char*)malloc(key_len + 1);
    for (int i = 0; i < key_len; i++)
        env->key[i] = key[i];
    env->key[key_len] = '\0';

    int val_len = str_len(value);
    env->value = (char*)malloc(val_len + 1);
    for (int i = 0; i < val_len; i++)
        env->value[i] = value[i];
    env->value[val_len] = '\0';

    env->next = NULL;
    return env;
}

t_env   *parse_envs(char** env)
{
    t_env* head = NULL;
    t_env* tail = NULL;

    while (*env)
    {
        int i = 0;
        while ((*env)[i] != '=' && (*env)[i] != '\0') i++;
        if ((*env)[i] == '=') {
            (*env)[i] = '\0';
            t_env* new_node = new_env(*env, *env + i + 1);
            if (!head)
                head = new_node;
            else
                tail->next = new_node;
            tail = new_node;
            (*env)[i] = '=';
        }
        env++;
    }
    return head;
}

char* get_env_val(t_env* envs, char* key)
{
    t_env* current = envs;
    while (current)
    {
        int i = 0;
        while (current->key[i] == key[i] && current->key[i] != '\0')
            i++;
        if (current->key[i] == key[i])
            return current->value;
        current = current->next;
    }
    return NULL;
}

char* append_to_str(char* dest, char* src)
{
    int dest_len = str_len(dest);
    int src_len = str_len(src);
    char* new_str = (char*)malloc(dest_len + src_len + 1);
    
    for (int i = 0; i < dest_len; i++)
        new_str[i] = dest[i];
    for (int i = 0; i < src_len; i++)
        new_str[dest_len + i] = src[i];
    new_str[dest_len + src_len] = '\0';

    return new_str;
}

char    *expand_result(char* str, char** env)
{
    int i = 0, start = 0;
    char *result = NULL;
    bool should_expand = true;

    while (str[i] != '\0')
    {
        if (str[i] == '\'')
        {
            should_expand = !should_expand; // Toggle on encountering single quote
            i++;
            continue;
        }

        if (str[i] == '$' && should_expand)
        {
            if (i != start)
            {
                char* temp = (char*)malloc(i - start + 1);
                for (int j = start; j < i; j++)
                    temp[j - start] = str[j];
                temp[i - start] = '\0';
                if (result != NULL)
                {
                    char* oldResult = result;
                    result = append_to_str(oldResult, temp);
                    free(oldResult);
                    free(temp);
                }
                else
                    result = temp;
            }
            start = ++i;
            while (str[i] != '\0' && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_'))
                i++;
            if (i != start)
            {
                char *temp = (char*)malloc(i - start + 1);
                for (int j = start; j < i; j++)
                    temp[j - start] = str[j];
                temp[i - start] = '\0';

                char* value = NULL;
                for (int j = 0; env[j] != NULL; j++)
                {
                    if (strncmp(temp, env[j], i - start) == 0 && env[j][i - start] == '=')
                    {
                        value = env[j] + i - start + 1;
                        break;
                    }
                }

                if (value != NULL) {
                    if (result != NULL) {
                        char* oldResult = result;
                        result = append_to_str(oldResult, value);
                        free(oldResult);
                    } else
                    {
                        result = (char*)malloc(str_len(value) + 1);
                        for (int j = 0; j <= str_len(value); j++)
                            result[j] = value[j];
                    }
                }
                free(temp);
            }
            start = i;
        }
        else
            i++;
    }

    if (i != start)
    {
        char* temp = (char*)malloc(i - start + 1);
        for (int j = start; j < i; j++)
            temp[j - start] = str[j];
        temp[i - start] = '\0';

        if (result != NULL) {
            char* oldResult = result;
            result = append_to_str(oldResult, temp);
            free(oldResult);
            free(temp);
        }
        else
            result = temp;
    }
    return result;
}

int main(int ac, char* av[], char* env[]) {
    (void)ac;
    (void)av;
    char* result = expand_result("jasd\"$USER\";hjsa", env);
    printf("%s\n", result);
    free(result);
    return 0;
}
