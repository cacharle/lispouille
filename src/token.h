#ifndef LISPOUILLE_TOKEN_H
#define LISPOUILLE_TOKEN_H

enum token_tag
{
    TOK_UNDEFINED,
    TOK_STRING,
    TOK_INTEGER,
    TOK_REAL,
    TOK_SYMBOL,
    TOK_PARENT_OPEN,
    TOK_PARENT_CLOSE,
};

typedef struct token
{
    struct token  *next;
    enum token_tag tag;
    union
    {
        char  *str;
        long   integer;
        double real;
    };
} token_t;

token_t *
token_new(void);
void
token_destroy(token_t *token);
void
token_push_front(token_t **root, token_t *token);
void
token_reverse(token_t **root);
void
token_print(token_t *token);

#endif
