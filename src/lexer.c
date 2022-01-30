#define _POSIX_C_SOURCE 200809L
#include "token.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t *
lex_number(char **input_ptr)
{
    char    *input = *input_ptr;
    token_t *token = token_new();
    bool     is_real = false;
    char    *end = input;
    if (*input == '-')
        end++;
    bool is_hex = strncmp(end, "0x", 2) == 0;
    if (is_hex)
        end += 2;
    bool is_octal = !is_hex && *end == '0';
    while (*end != '\0')
    {
        if (is_hex)
        {
            if (!isxdigit(*end))
                break;
        }
        else if (is_octal)
        {
            if (!isdigit(*end))
                break;
        }
        else
        {
            if (!isdigit(*end) && *end != '.')
                break;
        }
        if (*end == '.')
        {
            if (is_real)
            {
                fputs("Multiple '.' in number\n", stderr);
                abort();
            }
            is_real = true;
        }
        end++;
    }
    token->tag = is_real ? TOK_REAL : TOK_INTEGER;
    if (is_real)
        token->real = strtod(input, NULL);
    else
        // base=0 means it will interpret base 16 if 0x,
        // base 8 if 0... and base 10 otherwise
        token->integer = strtol(input, NULL, 0);
    *input_ptr = end;
    return token;
}

token_t *
lex_string(char **input_ptr)
{
    char *input = *input_ptr + 1;
    char *end = input;
    for (; *end != '\0' && *end != '"'; end++)
    {
        if (end[0] == '\\' && end[1] != '\0')
            end++;
    }
    if (*end == '\0')
    {
        fputs("No closing '\"' for string", stderr);
        abort();
    }
    token_t *token = token_new();
    token->tag = TOK_STRING;
    size_t str_size = end - input;
    token->str = strndup(input, str_size + 1);
    token->str[str_size] = '\0';
    *input_ptr = end + 1;
    for (size_t i = 0; token->str[i] != '\0'; i++)
    {
        if (token->str[i] == '\\' && token->str[i + 1] != '\0')
        {
            char  *rest = token->str + i + 1;
            size_t rest_len = strlen(rest);
            memmove(token->str + i, rest, rest_len);
            token->str[i + rest_len] = '\0';
            switch (token->str[i])
            {
            case 't': token->str[i] = '\t'; break;
            case 'n': token->str[i] = '\n'; break;
            case 'r': token->str[i] = '\r'; break;
            case 'v': token->str[i] = '\v'; break;
            case 'f': token->str[i] = '\f'; break;
            }
        }
    }
    return token;
}

token_t *
lex(char *input)
{
    token_t *root = NULL;

    while (*input != '\0')
    {
        if (isspace(*input))
            input++;
        if (isdigit(*input) || *input == '-')
        {
            token_t *token = lex_number(&input);
            token_push_front(&root, token);
        }
        else if (*input == '"')
        {
            token_t *token = lex_string(&input);
            token_push_front(&root, token);
        }
        else if (*input == '(')
        {
            token_t *token = token_new();
            token->tag = TOK_PARENT_OPEN;
            token_push_front(&root, token);
            input++;
        }
        else if (*input == ')')
        {
            token_t *token = token_new();
            token->tag = TOK_PARENT_CLOSE;
            token_push_front(&root, token);
            input++;
        }
        else if (isprint(*input) && !isspace(*input))
        {
        }
    }
    return root;
}
