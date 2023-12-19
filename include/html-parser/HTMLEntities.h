#ifndef _MENCI_HTML_PARSER_HTMLENTITIES_H
#define _MENCI_HTML_PARSER_HTMLENTITIES_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

#include <string>

namespace html_parser {
	std::string decodeHTMLEntites(const std::string& text);
}

#endif // _MENCI_HTML_PARSER_HTMLENTITIES_H
