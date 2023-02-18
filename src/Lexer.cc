#include "Lexer.h"

#include <boost/regex.hpp>
#include <iostream>
#include <string>

std::vector<Token> getTokens(const StringEx& htmlContents) {
	static const boost::regex reTagLike(
		R"(<(?:(?:!--(?:[\S\s]*?)--)|(?:\/?[a-zA-Z][^>\s]*)\s*(?:[^>\s][^>\s=]*(?:\s*=\s*(?:(?:\"[\S\s]*?\")|(?:'[\S\s]*?')|(?:[^\s>]+)))?\s*)*?)>)"
	);

	std::vector<Token> tokens;

	// Because we assume that the HTML file is UTF-8 encoded, so if the HTML contents
	// have UTF-8 BOM (Byte Order Mark), just ignore it.
	static const StringEx byteOrderMark = "\xEF\xBB\xBF";
	bool hasBOM = htmlContents.startsWith(byteOrderMark);

	std::string::const_iterator searchStart = hasBOM
		? htmlContents.cbegin() + byteOrderMark.length()
		: htmlContents.cbegin();

	boost::match_results<std::string::const_iterator> match;
	while (boost::regex_search(searchStart, htmlContents.cend(), match, reTagLike)) {
		searchStart = match.suffix().first;

		auto unmatched = match.prefix();
		if (unmatched.length() > 0) {
			// Some text found before a match.
			tokens.emplace_back(Token::Type::Text, unmatched.str());
		}

		auto matched = match[0];
		tokens.emplace_back(Token::Type::TagLike, matched.str());
	}

	// Some contents unmatched in the end, add a text token.
	if (searchStart != htmlContents.cend()) {
		tokens.emplace_back(Token::Type::Text, StringEx(searchStart, htmlContents.cend()));
	}

	return tokens;
}
