#include <criterion/criterion.h>
#include "lexer.h"
#include "token.h"

Test(lex, integer)
{
    token_t *token = lex("123");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, 123);
}

Test(lex, integer_negative)
{
    token_t *token = lex("-123");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, -123);
}

Test(lex, real)
{
    token_t *token = lex("123.123");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_REAL);
    cr_assert_eq(token->real, 123.123);
}

Test(lex, real_negative)
{
    token_t *token = lex("-123.123");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_REAL);
    cr_assert_eq(token->real, -123.123);
}

Test(lex, integer_hex)
{
    token_t *token = lex("0x2abcdef");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, 0x2abcdef);
}

Test(lex, integer_hex_negative)
{
    token_t *token = lex("-0x2abcdef");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, -0x2abcdef);
}

Test(lex, integer_octal)
{
    token_t *token = lex("0755");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, 0755, "expected %ld actual %ld", 0755L, token->integer);
}

Test(lex, integer_octal_negative)
{
    token_t *token = lex("-0755");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(token->integer, -0755, "expected %ld actual %ld", -0755L, token->integer);
}
