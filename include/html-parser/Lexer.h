#ifndef _MENCI_HTML_PARSER_LEXER_H
#define _MENCI_HTML_PARSER_LEXER_H

#include <vector>
#include <string>
#include "macros.h"

#include "Token.h"

std::vector<Token> getTokens(const std::string &htmlContents);

#endif // _MENCI_HTML_PARSER_LEXER_H
