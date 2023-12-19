#ifndef _MENCI_HTML_PARSER_HTMLDOCUMENT_H
#define _MENCI_HTML_PARSER_HTMLDOCUMENT_H

#include <istream>
#include <memory>
#include <type_traits>
#include <string>

#include "macros.h"

#include "Lexer.h"
#include "DOM.h"
#include "DOMBuilder.h"
#include "DOMInspector.h"

namespace html_parser {
	class HTMLDocument
	{
		DOM::RootNode domRoot;

	public:
		class Element
		{
			friend class HTMLDocument;

			const std::shared_ptr<DOM::ElementNode> node;

			void checkNotNull() const
			{
				if (node == nullptr)
				{
					throw std::invalid_argument("Element is null");
				}
			}

		public:
			Element() = default;
			Element(const Element&) = default;
			Element(Element&&) = default;
			explicit Element(const std::shared_ptr<DOM::ElementNode> node) : node(node) {}

			void inspect() const
			{
				checkNotNull();
				inspectNode(node.get());
			}

			std::string getTextContent() const
			{
				checkNotNull();
				std::string result;
				HTMLDocument::internelGetTextContent(node.get(), result);
				return result;
			}

			std::string getDirectTextContent() const
			{
				checkNotNull();
				std::string result;
				HTMLDocument::internelGetDirectTextContent(node.get(), result);
				return result;
			}

			std::string getAttribute(const std::string& name) const
			{
				checkNotNull();
				return HTMLDocument::internelGetAttribute(node.get(), name);
			}

			std::vector<Element> getElementsByTagName(const std::string& tagName) const
			{
				checkNotNull();
				std::vector<Element> result;
				HTMLDocument::internelGetElementsByTagName(node.get(), tagName, result);
				return result;
			}

			std::vector<Element> getElementsByClassName(const std::string& className) const
			{
				checkNotNull();
				std::vector<Element> result;
				HTMLDocument::internelGetElementsByClassName(node.get(), className, result);
				return result;
			}

			std::vector<Element> getChildren() const
			{
				checkNotNull();
				std::vector<Element> result;
				HTMLDocument::internelGetChildren(node.get(), result);
				return result;
			}

			std::unordered_set<std::string> getClassList()
			{
				return this->node->classList;
			}

			std::unordered_set<std::string> getIdList()
			{
				return this->node->idList;
			}

			std::string getTagName()
			{
				return this->node->tagName;
			}

			operator bool() const
			{
				return node != nullptr;
			}
		};

	private:
		static Element internelGetElementById(const DOM::NodeWithChildren* node, const std::string& id);
		static void internelGetElementsByName(const DOM::NodeWithChildren* node, const std::string& name, std::vector<Element>& result);
		static void internelGetElementsByTagName(const DOM::NodeWithChildren* node, const std::string& tagName, std::vector<Element>& result);
		static void internelGetElementsByClassName(const DOM::NodeWithChildren* node, const std::string& className, std::vector<Element>& result);
		static void internelGetTextContent(const DOM::NodeWithChildren* node, std::string& result);
		static void internelGetDirectTextContent(const DOM::NodeWithChildren* node, std::string& result);
		static void internelGetChildren(const DOM::NodeWithChildren* node, std::vector<Element>& result);
		static std::string internelGetAttribute(const DOM::ElementNode* node, const std::string& name);

	public:
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<std::istream, std::remove_reference_t<T>>>>
		explicit HTMLDocument(T&& is) : HTMLDocument(std::string(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>())) {}

		explicit HTMLDocument(const std::string& html)
		{
			parse(html);
		}

		void parse(const std::string& html)
		{
			domRoot = buildDOM(getTokens(html));
		}

		void inspect() const
		{
			inspectNode(&domRoot);
		}

		std::string getTextContent() const
		{
			std::string result;
			HTMLDocument::internelGetTextContent(&domRoot, result);
			return result;
		}

		std::string getDirectTextContent() const
		{
			std::string result;
			HTMLDocument::internelGetDirectTextContent(&domRoot, result);
			return result;
		}

		Element getElementById(const std::string& id) const
		{
			return HTMLDocument::internelGetElementById(&domRoot, id);
		}

		std::vector<Element> getElementsByName(const std::string& name) const
		{
			std::vector<Element> result;
			HTMLDocument::internelGetElementsByName(&domRoot, name, result);
			return result;
		}

		std::vector<Element> getElementsByTagName(const std::string& tagName) const
		{
			std::vector<Element> result;
			HTMLDocument::internelGetElementsByTagName(&domRoot, tagName, result);
			return result;
		}

		std::vector<Element> getElementsByClassName(const std::string& className) const
		{
			std::vector<Element> result;
			HTMLDocument::internelGetElementsByClassName(&domRoot, className, result);
			return result;
		}

		std::vector<Element> getChildren() const
		{
			std::vector<Element> result;
			HTMLDocument::internelGetChildren(&domRoot, result);
			return result;
		}
	};
}

#endif // _MENCI_HTML_PARSER_HTMLDOCUMENT_H
