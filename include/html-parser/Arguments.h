#ifndef _MENCI_HTML_PARSER_ARGUMENTS_H
#define _MENCI_HTML_PARSER_ARGUMENTS_H

#include <string>

namespace html_parser {
    struct Arguments
    {
        std::string inputFile;
    };

    Arguments parseArguments(int argc, char* argv[]);
}

#endif // _MENCI_HTML_PARSER_ARGUMENTS_H
