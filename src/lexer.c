#include <stdlib.h>

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

struct token
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
};

// static enum token_tag current_tag;

struct token *
lex(char *input)
{
    struct tocken *token = token_new();

    while (*input != '\0')
    {
        switch (token->tag)
        {
            case TOK_UNDEFINED:
                if (isdigit(*input) || *input == '-')
                {
                    bool is_real = false;
                    char *end = input;
                    if (strncmp(end, "0x") == 0)
                        end += 2;
                    while (*end != '\0' && (isdigit(*end) || *end == '.'))
                    {
                        if (*end == '.')
                        {
                            if (is_real)
                                abort(); // error, already encoutered
                            is_real = true;
                        }
                        end++;
                    }
                    token->tag = is_real ? TOK_REAL : TOK_INTEGER;
                    if (is_real)
                        token->real = strtod(input, NULL);
                    else
                        token->integer = strtol(input, NULL, 0);  // base=0 means it will interpret base 16 if 0x, base 8 if 0... and base 10 otherwise
                    input = end;
                }
                break;
            case TOK_STRING:
                break;
        }
    }

    return NULL;
}
