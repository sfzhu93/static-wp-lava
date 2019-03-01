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

        Node(): name(value) {}
        Node(Node& node): Node()
        {
            this->type = node.type;
            if (node.left != nullptr)
            {
                this->left = std::make_shared<Node>(*node.left);
            }else{
                this->left = nullptr;
            }
            if (node.right != nullptr)
            {
                this->right = std::make_shared<Node>(*node.right);
            }else{
                this->right = nullptr;
            }
            this->value = node.value;

        }

        Node(NodeType type, NodePtr&& left, NodePtr&& right, std::string&& val):
            type(type), left(std::move(left)), right(std::move(right)), value(std::move(val)), name(value)
        {

        }

        std::string ToString()
        {
            std::string ret;
            switch(this->type)
            {
                case UNIOP:
                    ret = "(" + this->value + " " + this->left->ToString() + ")";
                    break;
                case BINOP:
                    ret = "(" + this->left->ToString() + " " + this->value + " " + this->right->ToString() + ")";
                    break;
                case CONST:
                case VAR:
                    return this->value;
            }
            return ret;
        }

        static NodePtr CreateBinOp(NodePtr&& left, NodePtr&& right, std::string&& value)
        {
            auto ret = std::make_shared<Node>(BINOP, std::move(left), std::move(right), std::move(value));
            return ret;
        }

        static NodePtr CreateUniOp(NodePtr&& left, std::string&& value)
        {
            auto ret = std::make_shared<Node>(UNIOP, std::move(left), std::move(std::make_shared<Node>()), std::move(value));
            return ret;
        }

        static NodePtr CreateVar(std::string&& name)
        {
            auto ret = std::make_shared<Node>(VAR, std::move(std::make_shared<Node>()), std::move(std::make_shared<Node>()), std::move(name));
            return ret;
        }

        static NodePtr CreateConst(std::string value)
        {
            return std::make_shared<Node>(CONST, std::move(std::make_shared<Node>()), std::move(std::make_shared<Node>()), std::move(value));
        }

        static NodePtr substitute(NodePtr& src, std::string&& name, NodePtr expr){
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
                    substitute(src->left, std::move(name), expr);
                    break;
                case BINOP:
                    substitute(src->left, std::move(name), expr);
                    substitute(src->right, std::move(name), expr);
                    break;
            }
            return src;
        }

    private:

    };
}



#endif //STATIC_WP_LAVA_WPEXPR_H
