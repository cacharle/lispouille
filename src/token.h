#ifndef LISPOUILLE_TOKEN_H
#define LISPOUILLE_TOKEN_H

enum token_tag
{
    TOK_UNDEFINED,
    TOK_STRING,
    TOK_INTEGER ,
    TOK_REAL,
    TOK_SYM,
    TOK_PARENT_OPEN,
    TOK_PARENT_CLOSE,
};

typedef struct token
{
    struct token *next;
    enum token_tag tag;
    union
    {
        char *str;
        char *sym;
        long integer;
        double real;
    };
} token_t;

token_t *
token_new(void);

#endif
