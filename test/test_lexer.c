#include "lexer.h"
#include "token.h"
#include <criterion/criterion.h>

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
    cr_assert_eq(
        token->integer, 0755, "expected %ld actual %ld", 0755L, token->integer);
}

Test(lex, integer_octal_negative)
{
    token_t *token = lex("-0755");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_INTEGER);
    cr_assert_eq(
        token->integer, -0755, "expected %ld actual %ld", -0755L, token->integer);
}

Test(lex, string)
{
    token_t *token = lex("\"foo\"");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_STRING);
    cr_assert_str_eq(token->str, "foo");
}

Test(lex, string_escape)
{
    token_t *token = lex("\"fo\\o\"");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_STRING);
    cr_assert_str_eq(token->str, "foo");
    token = lex("\"f\\o\\o\"");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_STRING);
    cr_assert_str_eq(token->str, "foo");
    token = lex("\"f\\\\\\\\\"");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_STRING);
    cr_assert_str_eq(token->str, "f\\\\");
}

Test(lex, string_escape_special)
{
    token_t *token = lex("\"\\t\\n\\r\\v\\f\"");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_STRING);
    cr_assert_str_eq(token->str, "\t\n\r\v\f");
}

Test(lex, parent_open)
{
    token_t *token = lex("(");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_PARENT_OPEN);
}

Test(lex, parent_close)
{
    token_t *token = lex(")");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_PARENT_CLOSE);
}

Test(lex, symbol)
{
    token_t *token = lex("foo");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_SYMBOL);
    cr_assert_str_eq(token->str, "foo");
}

Test(lex, symbol_no_parent)
{
    token_t *token = lex("foo)");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_SYMBOL);
    cr_assert_str_eq(token->str, "foo");
    token = lex("foo(");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_SYMBOL);
    cr_assert_str_eq(token->str, "foo");
}

Test(lex, symbol_non_alpha_char)
{
    token_t *token = lex("foo->bar");
    cr_assert_not_null(token);
    cr_assert_eq(token->tag, TOK_SYMBOL);
    cr_assert_str_eq(token->str, "foo->bar");
}

Test(lex, multiple_integers)
{
    token_t *token = lex("1234 0xff 0644 -1234 -0xee -0344");
    long     integers[] = {1234, 0xff, 0644, -1234, -0xee, -0344};
    for (size_t i = 0; i < sizeof(integers) / sizeof(integers[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, TOK_INTEGER);
        cr_assert_eq(token->integer,
                     integers[i],
                     "%zu: actual %ld != expected %ld",
                     i,
                     token->integer,
                     integers[i]);
        token = token->next;
    }
}

Test(lex, multiple_real)
{
    token_t *token = lex("12.34 -12.34");
    double   reals[] = {12.34, -12.34};
    for (size_t i = 0; i < sizeof(reals) / sizeof(reals[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, TOK_REAL);
        cr_assert_eq(token->real,
                     reals[i],
                     "%zu: actual %f != expected %f",
                     i,
                     token->real,
                     reals[i]);
        token = token->next;
    }
}

Test(lex, multiple_numbers)
{
    token_t       *token = lex("12.34 1234 0xff 0644 -12.34 -1234 -0xee -0344");
    enum token_tag tags[] = {TOK_REAL,
                             TOK_INTEGER,
                             TOK_INTEGER,
                             TOK_INTEGER,
                             TOK_REAL,
                             TOK_INTEGER,
                             TOK_INTEGER,
                             TOK_INTEGER};
    for (size_t i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, tags[i]);
        token = token->next;
    }
}

Test(lex, multiple_strings)
{
    token_t *token = lex("\"foo\" \"bar\" \"baz\"");
    char    *strings[] = {"foo", "bar", "baz"};
    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, TOK_STRING);
        cr_assert_str_eq(token->str,
                         strings[i],
                         "%zu: actual %s != expected %s",
                         i,
                         token->str,
                         strings[i]);
        token = token->next;
    }
}

Test(lex, multiple_symbols)
{
    token_t *token = lex("foo bar baz");
    char    *strings[] = {"foo", "bar", "baz"};
    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, TOK_SYMBOL);
        cr_assert_str_eq(token->str,
                         strings[i],
                         "%zu: actual %s != expected %s",
                         i,
                         token->str,
                         strings[i]);
        token = token->next;
    }
}

Test(lex, multiple_anything)
{
    token_t       *token = lex("( foo \"hello\" 3.14 bar 1234 baz 0xaa \"bye\" )");
    enum token_tag tags[] = {
        TOK_PARENT_OPEN,
        TOK_SYMBOL,
        TOK_STRING,
        TOK_REAL,
        TOK_SYMBOL,
        TOK_INTEGER,
        TOK_SYMBOL,
        TOK_INTEGER,
        TOK_STRING,
        TOK_PARENT_CLOSE,
    };
    for (size_t i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, tags[i]);
        token = token->next;
    }
}

Test(lex, parent_no_space)
{
    token_t       *token = lex("(((yes)))");
    enum token_tag tags[] = {
        TOK_PARENT_OPEN,
        TOK_PARENT_OPEN,
        TOK_PARENT_OPEN,
        TOK_SYMBOL,
        TOK_PARENT_CLOSE,
        TOK_PARENT_CLOSE,
        TOK_PARENT_CLOSE,
    };
    token_print(token);
    for (size_t i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, tags[i], "%zu", i);
        token = token->next;
    }
}

Test(lex, form)
{
    token_t       *token = lex("(filter (lambda (x) (even x)) (cdr xs))");
    enum token_tag tags[] = {
        TOK_PARENT_OPEN,  // filter >
        TOK_SYMBOL,
        TOK_PARENT_OPEN,  // lambda >
        TOK_SYMBOL,
        TOK_PARENT_OPEN,  // (x) >
        TOK_SYMBOL,
        TOK_PARENT_CLOSE,  // (x) <
        TOK_PARENT_OPEN,   // (even x) >
        TOK_SYMBOL,
        TOK_SYMBOL,
        TOK_PARENT_CLOSE,  // (even x) <
        TOK_PARENT_CLOSE,  // lambda <
        TOK_PARENT_OPEN,   // (cdr xs) >
        TOK_SYMBOL,
        TOK_SYMBOL,
        TOK_PARENT_CLOSE,  // (cdr xs) <
        TOK_PARENT_CLOSE,  // filter <
    };
    for (size_t i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
    {
        cr_assert_not_null(token);
        cr_assert_eq(token->tag, tags[i], "%zu", i);
        token = token->next;
    }
}
