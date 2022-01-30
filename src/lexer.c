#include "token.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t *
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
                bool  is_real = false;
                char *end = input;
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
                input = end;
            }
            break;
        case TOK_STRING:
            break;

        default:
            fputs("Undefined token tag\n", stderr);
            abort();
        }
    }

    return token;
}
