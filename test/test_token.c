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
    token_destroy(token);
}
