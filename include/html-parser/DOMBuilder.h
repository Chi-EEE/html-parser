#ifndef _MENCI_HTML_PARSER_DOMBUILDER_H
#define _MENCI_HTML_PARSER_DOMBUILDER_H

#include <string>
#include "macros.h"

#include "Token.h"
#include "DOM.h"

namespace html_parser {
	DOM::RootNode buildDOM(const std::vector<Token>& tokens);
}

#endif // _MENCI_HTML_PARSER_DOMBUILDER_H
