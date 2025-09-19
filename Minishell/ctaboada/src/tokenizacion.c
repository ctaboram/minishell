
#include "../includes/minishell.h"

t_token *create_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = ft_strdup(value); // Duplicar la cadena
    token->type = type;
    token->next = NULL;
    return (token);
}

void add_token(t_token **list, t_token *new_token)
{
    t_token *tmp;

    if (!*list)
        *list = new_token;
    else
    {
        tmp = *list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
}

t_token *tokenize(char *line)
{
    t_token *tokens = NULL;
    char *start = line;
    char *end;

    while (*start)
    {
        // Saltar espacios
        while (*start && isspace(*start))
            start++;
        if (!*start)
            break;

        end = start;

        // Manejar operadores
        if (*end == '|')
        {
            add_token(&tokens, create_token("|", TOKEN_PIPE));
            start++;
        }
        else if (*end == '>' && *(end + 1) == '>')
        {
            add_token(&tokens, create_token(">>", TOKEN_APPEND));
            start += 2;
        }
        else if (*end == '>')
        {
            add_token(&tokens, create_token(">", TOKEN_REDIRECT_OUT));
            start++;
        }
        else if (*end == '<')
        {
            add_token(&tokens, create_token("<", TOKEN_REDIRECT_IN));
            start++;
        }
        else if (*end == '\'' || *end == '"')
        {
            char quote = *end;
            end++;
            while (*end && *end != quote)
                end++;
            if (*end == quote)
            {
                char *value = strndup(start + 1, end - start - 1);
                add_token(&tokens, create_token(value, TOKEN_QUOTE));
                free(value);
                end++;
            }
            start = end;
        }
        else
        {
            // Extraer palabras (comandos o argumentos)
            while (*end && !isspace(*end) && *end != '|' && *end != '>' && *end != '<')
                end++;
            char *value = strndup(start, end - start);
            add_token(&tokens, create_token(value, TOKEN_WORD));
            free(value);
            start = end;
        }
    }
    add_token(&tokens, create_token("", TOKEN_EOF)); // Marca el fin
    return (tokens);
}
