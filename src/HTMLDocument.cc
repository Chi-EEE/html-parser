#include "HTMLDocument.h"

#include <exception>

HTMLDocument::Element HTMLDocument::internelGetElementById(const DOM::NodeWithChildren *node, const std::string &id)
{
	if (id.empty())
	{
		throw std::logic_error("Empty string passed to getElementById()");
	}

	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			if (childElement->idList.find(id) != childElement->idList.end())
			{
				return HTMLDocument::Element(childElement);
			}

			if (HTMLDocument::Element element = HTMLDocument::internelGetElementById(childElement.get(), id))
			{
				return element;
			}
		}
	}

	return HTMLDocument::Element();
}

void HTMLDocument::internelGetElementsByName(const DOM::NodeWithChildren *node,
											 const std::string &name,
											 std::vector<HTMLDocument::Element> &result)
{
	if (name.empty())
	{
		throw std::logic_error("Empty string passed to getElementsByName()");
	}

	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			if (HTMLDocument::internelGetAttribute(childElement.get(), "name") == name)
			{
				result.push_back(HTMLDocument::Element(childElement));
			}

			HTMLDocument::internelGetElementsByName(childElement.get(), name, result);
		}
	}
}

void HTMLDocument::internelGetElementsByTagName(const DOM::NodeWithChildren *node,
												const std::string &tagName,
												std::vector<HTMLDocument::Element> &result)
{
	if (tagName.empty())
	{
		throw std::logic_error("Empty string passed to getElementsByTagName()");
	}

	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			if (childElement->tagName == tagName)
			{
				result.push_back(HTMLDocument::Element(childElement));
			}

			HTMLDocument::internelGetElementsByTagName(childElement.get(), tagName, result);
		}
	}
}

void HTMLDocument::internelGetElementsByClassName(const DOM::NodeWithChildren *node,
												  const std::string &className,
												  std::vector<HTMLDocument::Element> &result)
{
	if (className.empty())
	{
		throw std::logic_error("Empty string passed to getElementsByName()");
	}

	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			if (childElement->classList.find(className) != childElement->classList.end())
			{
				result.push_back(HTMLDocument::Element(childElement));
			}

			HTMLDocument::internelGetElementsByClassName(childElement.get(), className, result);
		}
	}
}

void HTMLDocument::internelGetChildren(const DOM::NodeWithChildren *node, std::vector<Element> &result)
{
	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			result.push_back(HTMLDocument::Element(childElement));
		}
	}
}

void HTMLDocument::internelGetTextContent(const DOM::NodeWithChildren *node, std::string &result)
{
	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childElement = std::dynamic_pointer_cast<DOM::ElementNode>(child))
		{
			HTMLDocument::internelGetTextContent(childElement.get(), result);
		}
		else if (auto childTextNode = std::dynamic_pointer_cast<DOM::TextNode>(child))
		{
			result += childTextNode->content;
		}
	}
}

void HTMLDocument::internelGetDirectTextContent(const DOM::NodeWithChildren *node, std::string &result)
{
	for (const std::shared_ptr<DOM::Node> &child : node->children)
	{
		if (auto childTextNode = std::dynamic_pointer_cast<DOM::TextNode>(child))
		{
			result += childTextNode->content;
		}
	}
}

std::string HTMLDocument::internelGetAttribute(const DOM::ElementNode *node, const std::string &name)
{
	std::string clonedName = to_lower(name);
	std::unordered_map<std::string, std::string>::const_iterator it = node->attributes.find(clonedName);
	if (it == node->attributes.end())
		return "";
	return it->second;
}