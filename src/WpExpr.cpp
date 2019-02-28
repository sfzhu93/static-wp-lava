//
// Created by suz305 on 2/27/19.
//

#include "WpExpr.h"
namespace WpExpr{
    Node::NodePtr BinOp::substitute(std::string &name, Node::NodePtr node) {
        if (dynamic_cast<Var&>(*(this->left))
            this->left->substitute(name, node);
        this->right->substitute(name, node);
        return shared_from_this();
    }

}
