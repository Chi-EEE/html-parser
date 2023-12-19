#include "DOMInspector.h"

#include <iostream>
#include "TerminalColor.h"

namespace html_parser {
	std::string inspect(std::string data)
	{
		std::string result;
		for (char ch : data)
		{
			switch (ch)
			{
			case '"':
				result.append("\\\"");
				break;
			case '\?':
				result.append("\\?");
				break;
			case '\\':
				result.append("\\\\");
				break;
			case '\a':
				result.append("\\a");
				break;
			case '\b':
				result.append("\\b");
				break;
			case '\f':
				result.append("\\f");
				break;
			case '\n':
				result.append("\\n");
				break;
			case '\r':
				result.append("\\r");
				break;
			case '\t':
				result.append("\\t");
				break;
			case '\v':
				result.append("\\v");
				break;
			default:
				result.append(1, ch);
			}
		}
		return '"' + result + '"';
	}

	void inspectNode(const DOM::Node* node, size_t indentLevel)
	{
		auto indent = [indentLevel](int add = 0) -> std::string
			{
				return std::string(indentLevel * 4 + add, ' ');
			};

		if (auto rootNode = dynamic_cast<const DOM::RootNode*>(node))
		{
			(void)rootNode; // Disable GCC -Wunused-variable.
			std::cout << indent()
				<< TerminalColor::Bold << TerminalColor::ForegroundYellow
				<< "DOM::RootNode"
				<< TerminalColor::Reset
				<< std::endl;
		}
		else if (auto textNode = dynamic_cast<const DOM::TextNode*>(node))
		{
			std::cout << indent()
				<< TerminalColor::Bold << TerminalColor::ForegroundBlue
				<< "DOM::TextNode "
				<< TerminalColor::Reset << TerminalColor::ForegroundGreen
				<< inspect(textNode->content)
				<< TerminalColor::Reset
				<< std::endl;
		}
		else if (auto elementNode = dynamic_cast<const DOM::ElementNode*>(node))
		{
			std::cout << indent()
				<< TerminalColor::Bold << TerminalColor::ForegroundCyan
				<< "DOM::ElementNode"
				<< TerminalColor::Reset
				<< '('
				<< TerminalColor::Bold
				<< elementNode->tagName
				<< TerminalColor::Reset
				<< ')'
				<< std::endl;
			for (const std::pair<std::string, std::string>& attribute : elementNode->attributes)
			{
				std::cout << indent(1)
					<< '['
					<< TerminalColor::ForegroundYellow
					<< "Attribute "
					<< TerminalColor::Reset << TerminalColor::Bold
					<< inspect(attribute.first)
					<< TerminalColor::Reset
					<< " = "
					<< TerminalColor::ForegroundMagenta
					<< inspect(attribute.second)
					<< TerminalColor::Reset
					<< ']'
					<< std::endl;
			}
		}
		else if (auto commentNode = dynamic_cast<const DOM::CommentNode*>(node))
		{
			std::cout << indent()
				<< TerminalColor::Bold << TerminalColor::ForegroundMagenta
				<< "DOM::CommentNode "
				<< TerminalColor::Reset << TerminalColor::ForegroundGreen
				<< inspect(commentNode->content)
				<< TerminalColor::Reset
				<< std::endl;
		}

		if (auto nodeWithChildren = dynamic_cast<const DOM::NodeWithChildren*>(node))
		{
			for (const std::shared_ptr<DOM::Node>& childNode : nodeWithChildren->children)
			{
				inspectNode(childNode.get(), indentLevel + 1);
			}
		}
	}
}
