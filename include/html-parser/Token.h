#ifndef _MENCI_HTML_PARSER_TOKEN_H
#define _MENCI_HTML_PARSER_TOKEN_H

#include <string>

namespace html_parser {
    struct Token
    {
        enum Type
        {
            TagLike,
            Text
        } type;

        std::string content;

        Token(Type type, std::string content) : type(type), content(content) {}
    };
}

#endif // _MENCI_HTML_PARSER_TOKEN_H
