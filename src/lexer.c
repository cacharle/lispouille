#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"

struct token *
lex(char *input)
{
    token_t *token = token_new();

    while (*input != '\0')
    {
        switch (token->tag)
        {
            case TOK_UNDEFINED:
                if (isdigit(*input) || *input == '-')
                {
                    bool is_real = false;
                    char *end = input;
                    if (strncmp(end, "0x", 2) == 0)
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

            default:
                abort();
        }
    }

    return NULL;
}
