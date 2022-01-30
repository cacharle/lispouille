#include <stdlib.h>
#include "token.h"

token_t *
token_new(void)
{
    token_t *token = malloc(sizeof(token_t));
    token->tag = TOK_UNDEFINED;
    token->next = NULL;
    token->str = NULL;
    return token;
}
