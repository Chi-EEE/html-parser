#ifndef _MENCI_HTML_PARSER_TERMINALCOLOR_H
#define _MENCI_HTML_PARSER_TERMINALCOLOR_H

#include <ostream>

namespace html_parser {
    enum TerminalColor
    {
        Reset = 0,
        Bold = 1,

        ForegroundBlack = 30,
        ForegroundRed = 31,
        ForegroundGreen = 32,
        ForegroundYellow = 33,
        ForegroundBlue = 34,
        ForegroundMagenta = 35,
        ForegroundCyan = 36,
        ForegroundWhite = 37,

        BackgroundBlack = 40,
        BackgroundRed = 41,
        BackgroundGreen = 42,
        BackgroundYellow = 43,
        BackgroundBlue = 44,
        BackgroundMagenta = 45,
        BackgroundCyan = 46,
        BackgroundWhite = 47
    };

    static std::ostream& operator<<(std::ostream& os, TerminalColor tc)
    {
        return os << "\033[" << (int)tc << "m";
    }
}

#endif // _MENCI_HTML_PARSER_TERMINALCOLOR_H
