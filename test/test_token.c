#define _XOPEN_SOURCE 500
#include <string.h>
#include <criterion/criterion.h>
#include "token.h"

Test(token, new)
{
    token_t *token = token_new();
    cr_assert_not_null(token);
    cr_assert_null(token->next);
    cr_assert_null(token->str);
    cr_assert_eq(token->tag, TOK_UNDEFINED);
}

Test(token, destroy)
{
    token_t *token = token_new();
    token->next = token_new();
    token->next->next = token_new();
    token_destroy(token);
}

Test(token, destroy_string_symbol)
{
    token_t *token = token_new();
    token->tag = TOK_STRING;
    token->str = strdup("hello");
    token_destroy(token);
    token = token_new();
    token->tag = TOK_SYMBOL;
    token->str = strdup("hello");
    token_destroy(token);
}

Test(token, push_front)
{
    token_t *token = token_new();
    token_push_front(&token, token_new());
    token_push_front(&token, token_new());
    cr_assert_not_null(token);
    cr_assert_not_null(token->next);
    cr_assert_not_null(token->next->next);
    cr_assert_null(token->next->next->next);
}

Test(token, reverse)
{
    token_t *token = token_new();
    token_push_front(&token, token_new());
    token_push_front(&token, token_new());
    token_t *t1 = token;
    token_t *t2 = token->next;
    token_t *t3 = token->next->next;
    token_reverse(&token);
    cr_assert_eq(t1, token->next->next);
    cr_assert_eq(t2, token->next);
    cr_assert_eq(t3, token);
}

Test(token, reverse_null)
{
    token_t *token = NULL;
    token_reverse(&token);
    cr_assert_null(token);
}
