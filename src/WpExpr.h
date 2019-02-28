//
// Created by suz305 on 2/27/19.
//

#ifndef STATIC_WP_LAVA_WPEXPR_H
#define STATIC_WP_LAVA_WPEXPR_H

#include <memory>

namespace WpExpr{
    enum NodeType {BINOP, UNIOP, VAR, CONST};
    enum Optr {PLUS, MINUS, AND, OR, NOT, MUL, DIV};

    class Node : public std::enable_shared_from_this<Node> {
    public:
        NodeType type;
        typedef std::shared_ptr<Node> NodePtr;
        NodePtr left, right;
        std::string value;
        std::string& name;
        Node(NodeType type, NodePtr&& left, NodePtr&& right, std::string& value):
        type(type), left(left), right(right), value(value), name(value)
        {

        }

        static NodePtr CreateBinOp(NodePtr&& left, NodePtr&& right)
        {
            auto ret = std::make_shared<Node>(BINOP, left, right, "");
            return ret;
        }

        static NodePtr CreateUniOp(NodePtr&& left)
        {
            auto ret = std::make_shared<Node>(UNIOP, left, nullptr, "");
            return ret;
        }

        static NodePtr CreateVar(std::string& name)
        {
            auto ret = std::make_shared<Node>(VAR, nullptr, nullptr, name);
            return ret;
        }

        static NodePtr CreateConst(std::string& value)
        {
            return std::make_shared<Node>(CONST, nullptr, nullptr, value);
        }

        static NodePtr substitute(NodePtr& src, std::string& name, NodePtr& expr){
            switch(src->type)
            {
                case VAR:
                    if (src->name == name)
                    {
                        src = expr;
                    }else
                    {
                        //TODO: warning nothing substituted
                    }
                    break;
                case CONST:
                    //TODO: warning nothing substituted
                    break;
                case UNIOP:
                    substitute(src->left, name, expr);
                    break;
                case BINOP:
                    substitute(src->left, name, expr);
                    substitute(src->right, name, expr);
                    break;
            }
            return src;
        }

    private:

    };
}



#endif //STATIC_WP_LAVA_WPEXPR_H
