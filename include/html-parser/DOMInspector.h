#ifndef _MENCI_HTML_PARSER_DOMINSPECTOR_H
#define _MENCI_HTML_PARSER_DOMINSPECTOR_H

#include "DOM.h"
#include <string>

namespace html_parser {
	void inspectNode(const DOM::Node* node, size_t indentLevel = 0);
}

#endif // _MENCI_HTML_PARSER_DOMINSPECTOR_H
