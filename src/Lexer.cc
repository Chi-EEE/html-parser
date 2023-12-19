#include "Lexer.h"

#include <iostream>
#include <string>

namespace html_parser {
	std::vector<Token> getTokens(const std::string& htmlContents)
	{
		static const regex reTagLike(
			R"(<(?:(?:!--(?:[\S\s]*?)--)|(?:\/?[a-zA-Z][^>\s]*)\s*(?:[^>\s][^>\s=]*(?:\s*=\s*(?:(?:\"[\S\s]*?\")|(?:'[\S\s]*?')|(?:[^\s>]+)))?\s*)*?)>)");

		std::vector<Token> tokens;

		// Because we assume that the HTML file is UTF-8 encoded, so if the HTML contents
		// have UTF-8 BOM (Byte Order Mark), just ignore it.
		static const std::string byteOrderMark = "\xEF\xBB\xBF";
		bool hasBOM = starts_with(htmlContents, byteOrderMark);

		std::string::const_iterator searchStart = hasBOM
			? htmlContents.cbegin() + byteOrderMark.length()
			: htmlContents.cbegin();

		match_results match;
		while (regex_search(searchStart, htmlContents.cend(), match, reTagLike))
		{
			searchStart = match.suffix().first;

			auto unmatched = match.prefix();
			if (unmatched.length() > 0)
			{
				// Some text found before a match.
				tokens.emplace_back(Token::Type::Text, unmatched.str());
			}

			auto matched = match[0];
			tokens.emplace_back(Token::Type::TagLike, matched.str());
		}

		// Some contents unmatched in the end, add a text token.
		if (searchStart != htmlContents.cend())
		{
			tokens.emplace_back(Token::Type::Text, std::string(searchStart, htmlContents.cend()));
		}

		return tokens;
	}
}
