#include "lexer.h"
#include "token.h"

int
main()
{
    token_t *token = lex("(((313)))");

    token_print(token);
    return 0;
}
