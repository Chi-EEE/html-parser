#include "DOMBuilder.h"

#include "HTMLEntities.h"

// These elements' open tags don't need close tags and they don't have a subtree.
// From https://developer.mozilla.org/en-US/docs/Glossary/Empty_element.
static const char *const emptyElements[] = {
	"area",
	"base",
	"br",
	"col",
	"colgroup",
	"command",
	"embed",
	"hr",
	"img",
	"input",
	"keygen",
	"link",
	"meta",
	"param",
	"source",
	"track",
	"wbr"};

// These elements don't contain any child elements. Everything inside them is
// just a TextNode.
// From https://www.w3.org/TR/html5/syntax.html#writing-html-documents-elements.
static const char *const rawTextElements[] = {
	"script",
	"style",
	"textarea",
	"title"};

// These raw text elements contain escaped string -- e.g. &nbsp; will be replaced
// with a space ' ' in these elements but not in other raw text elements.
// From https://www.w3.org/TR/html5/syntax.html#writing-html-documents-elements.
static const char *const escapableRawTextElements[] = {
	"textarea",
	"title"};

inline void tokenize(std::string str, const char delimiter, std::vector<std::string> &out)
{
	int start = 0;
	int end = str.find(delimiter);
	while (end != -1)
	{
		out.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	out.push_back(str.substr(start, end - start));
}

static void parseTagOpen(const std::string &token, std::string &tagName, std::unordered_map<std::string, std::string> &attributes, std::unordered_set<std::string> &classList, std::unordered_set<std::string> &idList)
{
	// Extract tag name and all attributes.
	// ^<(\S+)\s*([\S\s]*)>$
	static const regex reTag(
		"^<(\\S+)\\s*([\\S\\s]*)>$");

	match_results match;
	if (!regex_search(token.begin(), token.end(), match, reTag))
	{
		throw std::logic_error("Failed to apply regex on a tag open token");
	}

	tagName = match[1].str();
	std::string attributesString = match[2].str();

	// HTML tag's name is case-insensitive.
	tagName = to_lower(tagName);

	// Extract all attributes.
	// ([^>\s][^>\s=]*)(?:\s*=\s*(?:(?:"([\S\s]*?)")|(?:'([\S\s]*?)')|([^\s>]+)))?\s*
	static const regex reAttribute(
		"([^>\\s][^>\\s=]*)(?:\\s*=\\s*(?:(?:\"([\\S\\s]*?)\")|(?:'([\\S\\s]*?)')|([^\\s>]+)))?\\s*");

	std::string::const_iterator searchStart = attributesString.cbegin();
	while (regex_search(searchStart, attributesString.cend(), match, reAttribute))
	{
		std::string key = match[1].str(),
					value;

		// A self-closing tag's close sign '/' is parsed to an attribute name
		if (key != "/")
		{
			// 3 types: <"str">, <'str'> and <str>, in capture group 2, 3 and 4.
			for (int i = 2; i <= 4; i++)
			{
				if (match[i].length() > 0)
				{
					value = match[i].str();
					break;
				}
			}

			// HTML tag's attribute's name is case-insensitive.
			key = to_lower(key);

			attributes[key] = value;
		}

		searchStart = match.suffix().first;
	}
	if (attributes.find("class") != attributes.end())
	{
		std::vector<std::string> classes;
		tokenize(attributes["class"], ' ', classes);

		for (int i = 0; i < classes.size(); i++)
		{
			classList.insert(classes[i]);
		}
		attributes.erase("class");
	}
	if (attributes.find("id") != attributes.end())
	{
		std::vector<std::string> ids;
		tokenize(attributes["id"], ' ', ids);

		for (int i = 0; i < ids.size(); i++)
		{
			idList.insert(ids[i]);
		}
		attributes.erase("id");
	}
}

static std::string parseTagClose(const std::string &token)
{
	// Extract tag name.
	static const regex reTagName("</(.+?)[\\s>]");

	match_results match;
	if (!regex_search(token.begin(), token.end(), match, reTagName))
	{
		throw std::logic_error("Failed to apply regex on a tag close token");
	}

	std::string tagName = match[1].str();

	tagName = to_lower(tagName);
	return tagName;
}

DOM::RootNode buildDOM(const std::vector<Token> &tokens)
{
	DOM::RootNode root;

	DOM::NodeWithChildren *currentNode = &root;
	for (const Token &token : tokens)
	{
		// Check if we're inside a raw text element.
		if (auto *currentElement = dynamic_cast<DOM::ElementNode *>(currentNode))
		{
			const std::string &tagName = currentElement->tagName;
			if (std::find(std::begin(rawTextElements),
						  std::end(rawTextElements),
						  tagName) != std::end(rawTextElements))
			{
				// Inside a raw text element, everything except a matching close tag is text.
				if (starts_with(token.content, "</") == 0 && parseTagClose(token.content) == tagName)
				{
					// OK, close it.
					currentNode = currentNode->parentNode;
				}
				else
				{
					// Append the token to the TextNode.
					if (currentElement->children.empty())
					{
						currentElement->children.push_back(std::make_shared<DOM::TextNode>(
							currentNode,
							""));
					}

					std::string &textContent = std::dynamic_pointer_cast<DOM::TextNode>(
												   currentElement->children[0])
												   ->content;

					std::string newText = token.content;
					if (std::find(std::begin(escapableRawTextElements),
								  std::end(escapableRawTextElements),
								  tagName) != std::end(escapableRawTextElements))
					{
						newText = decodeHTMLEntites(newText);
					}

					textContent += newText;
				}

				continue;
			}
		}

		static const std::string commentStart = "<!--",
								 commentEnd = "-->";
		switch (token.type)
		{
		case Token::Type::TagLike:
		{
			if (starts_with(token.content, commentStart) && ends_with(token.content, commentEnd))
			{
				// Comment
				currentNode->children.push_back(std::make_shared<DOM::CommentNode>(currentNode, token.content.substr(
																									commentStart.length(),
																									token.content.length() - commentStart.length() - commentEnd.length())));
			}
			else
			{
				// Tag open or tag close.
				if (!starts_with(token.content, "</"))
				{
					// Tag open.
					std::shared_ptr<DOM::ElementNode> tag = std::make_shared<DOM::ElementNode>(currentNode);
					parseTagOpen(token.content, tag->tagName, tag->attributes, tag->classList, tag->idList);

					currentNode->children.push_back(tag);

					if (std::find(std::begin(emptyElements),
								  std::end(emptyElements),
								  tag->tagName) == std::end(emptyElements))
					{
						// It's NOT a empty element. Open a new subtree.
						currentNode = tag.get();
					}
				}
				else
				{
					// Tag close.
					std::string tagName = parseTagClose(token.content);

					if (std::find(std::begin(emptyElements),
								  std::end(emptyElements),
								  tagName) != std::end(emptyElements))
					{
						// Someone silly is trying to close a empty element. Just ignore it.
					}
					else
					{
						// OK. Let's close it.

						DOM::NodeWithChildren *originalCurrentNode = currentNode;
						DOM::ElementNode *currentElement;
						while ((currentElement = dynamic_cast<DOM::ElementNode *>(currentNode)) != nullptr // The node IS a element
							   && currentElement->tagName != tagName)
						{ // and tag name is not matched.
							// Close current node and continue to check tag name if tag name is NOT matched.
							currentNode = currentNode->parentNode;
						}

						if (currentElement == nullptr)
						{
							// Someone silly is trying to close a non-exist tag. Just ignore it and go back.
							currentNode = originalCurrentNode;
						}
						else
						{
							// Close current element's node.
							currentNode = currentNode->parentNode;
						}
					}
				}
			}

			break;
		}
		case Token::Type::Text:
		{
			// <!DOCTYPE> is lexed to some text -- since we don't support it, just ignore it.
			std::string tokenContent = trim(token.content);
			tokenContent = to_lower(tokenContent);
			if (starts_with(tokenContent, "<!DOCTYPE"))
			{
				break;
			}

			currentNode->children.push_back(std::make_shared<DOM::TextNode>(
				currentNode,
				decodeHTMLEntites(token.content)));
			break;
		}
		default:
			break;
		}
	}

	return root;
}
