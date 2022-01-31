#include "token.h"
#include <stdio.h>
#include <stdlib.h>

token_t *
token_new(void)
{
    token_t *token = malloc(sizeof(token_t));
    token->tag = TOK_UNDEFINED;
    token->next = NULL;
    token->str = NULL;
    return token;
}

void
token_destroy(token_t *token)
{
    if (token->next != NULL)
        token_destroy(token->next);
    if (token->tag == TOK_STRING || token->tag == TOK_SYMBOL)
        free(token->str);
    free(token);
}

void
token_push_front(token_t **root, token_t *token)
{
    token->next = *root;
    *root = token;
}

static token_t *
reverse_rec(token_t *token)
{
    if (token->next == NULL)
        return token;
    token_t *reversed = reverse_rec(token->next);
    token->next->next = token;
    token->next = NULL;
    return reversed;
}

void
token_reverse(token_t **root)
{
    if (*root == NULL)
        return;
    *root = reverse_rec(*root);
}

static char *token_tag_repr_lookup[] = {
    [TOK_UNDEFINED] = "undefined",
    [TOK_STRING] = "string",
    [TOK_INTEGER] = "integer",
    [TOK_REAL] = "real",
    [TOK_SYMBOL] = "symbol",
    [TOK_PARENT_OPEN] = "parent_open",
    [TOK_PARENT_CLOSE] = "parent_close",
};

void
token_print(token_t *token)
{
    for (; token != NULL; token = token->next)
    {
        fputc('[', stdout);
        fputs(token_tag_repr_lookup[token->tag], stdout);
        switch (token->tag)
        {
        case TOK_SYMBOL:
        case TOK_STRING: printf(" \"%s\"", token->str); break;
        case TOK_INTEGER: printf(" %ld", token->integer); break;
        case TOK_REAL: printf(" %.2f", token->real); break;
        default: break;
        }
        fputc(']', stdout);
        if (token->next != NULL)
            fputc(' ', stdout);
    }
    fputc('\n', stdout);
}
