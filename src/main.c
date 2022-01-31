#include "lexer.h"
#include "token.h"

int
main()
{
    token_t *token = lex("(((yes)))");

    token_print(token);
    return 0;
}
