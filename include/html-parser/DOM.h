#ifndef _MENCI_HTML_PARSER_DOM_H
#define _MENCI_HTML_PARSER_DOM_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

#include <string>

namespace html_parser {
    namespace DOM
    {

        struct Node
        {
            struct NodeWithChildren* parentNode;

            Node(struct NodeWithChildren* parentNode) : parentNode(parentNode) {}

            // A class without virtual table can't be dymanic_cast-ed.
            virtual ~Node() {}
        };

        struct NodeWithChildren : Node
        {
            NodeWithChildren(NodeWithChildren* parentNode) : Node(parentNode) {}

            std::vector<std::shared_ptr<Node>> children;
        };

        struct RootNode : NodeWithChildren
        {
            RootNode() : NodeWithChildren(nullptr) {}
        };

        struct ElementNode : NodeWithChildren
        {
            ElementNode(NodeWithChildren* parentNode) : NodeWithChildren(parentNode) {}

            std::string tagName;
            std::unordered_map<std::string, std::string> attributes;
            std::unordered_set<std::string> classList;
            std::unordered_set<std::string> idList;
        };

        struct TextNode : Node
        {
            std::string content;

            TextNode(NodeWithChildren* parentNode, const std::string& content) : Node(parentNode), content(content) {}
        };

        struct CommentNode : Node
        {
            std::string content;

            CommentNode(NodeWithChildren* parentNode, const std::string& content) : Node(parentNode), content(content) {}
        };

    }
}

#endif // _MENCI_HTML_PARSER_DOM_H
