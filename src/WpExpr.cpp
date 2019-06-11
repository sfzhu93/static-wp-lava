//
// Created by suz305 on 2/27/19.
//

#include "WpExpr.h"
#include "Miscs.h"

/*namespace WpExpr{
    Node::NodePtr BinOp::substitute(std::string &name, Node::NodePtr node) {
        if (dynamic_cast<Var&>(*(this->left))
            this->left->substitute(name, node);
        this->right->substitute(name, node);
        return shared_from_this();
    }

}
*/

using namespace WpExpr;

WpExpr::Node::Node(WpExpr::Node &node) : Node() {
    this->type = node.type;
    if (node.left != nullptr) {
        this->left = std::make_shared<Node>(*node.left);
    } else {
        this->left = nullptr;
    }
    if (node.right != nullptr) {
        this->right = std::make_shared<Node>(*node.right);
    } else {
        this->right = nullptr;
    }
    this->value = node.value;
    this->valueObj = node.valueObj;
    this->operatorType = node.operatorType;
}

std::string WpExpr::Node::ToString() {
    std::string ret;
    switch (this->type) {
        case UNIOP:
            ret = std::string("(") + WpExprOperator2CStr(this->operatorType) + " " + this->left->ToString() + ")";
            break;
        case BINOP:
            ret = "(" + this->left->ToString() + " " + WpExprOperator2CStr(this->operatorType) + " " + this->right->ToString() + ")";
            break;
        case CONST:
            return this->name;
        case VAR: {
            if (this->valueObj) {
                //std::string tmp_name = ;
                return this->valueObj->getName();
            }else {
                return this->value;
            }
        }
        case UPRED:
            return "UPred(" + this->left->ToString() + ")";
    }
    return ret;
}

std::string WpExpr::Node::ToSMTLanguage() {
    std::string ret;
    switch (this->type) {
        case UNIOP:
            ret = std::string("(") + WpExprOperator2CStr(this->operatorType) + " " + this->left->ToSMTLanguage() + ")";
            break;
        case BINOP:
            ret = std::string("( ") + WpExprOperator2CStr(this->operatorType) + " " + this->left->ToSMTLanguage() + " " + this->right->ToSMTLanguage() +
                  ")";
            break;
        case CONST:
        case VAR:
            return this->ToString();
        case UPRED:
            return "UPred("+ this->left->ToSMTLanguage() + ")";
    }
    return ret;
}

NodePtr
WpExpr::Node::CreateBinOp(NodePtr &&left, NodePtr &&right, WpExpr::Operator optr) {
    
    auto ret = std::make_shared<Node>(BINOP, std::move(left), std::move(right), "");
    ret->operatorType = optr;
    return ret;
}

NodePtr WpExpr::Node::CreateUniOp(NodePtr &&left, WpExpr::Operator optr) {
    auto ret = std::make_shared<Node>(UNIOP, std::move(left), NodePtr(), "");
    ret->operatorType = optr;
    return ret;
}

NodePtr WpExpr::Node::CreateVar(llvm::Value *val) {
    auto name = val->getName();
    auto ret = std::make_shared<Node>(VAR, NodePtr(),
                                      NodePtr(), name);
    ret->valueObj = val;
    return ret;
}

NodePtr WpExpr::Node::CreateVar(std::string name) {
    auto ret = std::make_shared<Node>(VAR, NodePtr(),
                                      NodePtr(), std::move(name));
    ret->valueObj = nullptr;
    return ret;
}

NodePtr WpExpr::Node::CreateConst(std::string value) {
    return std::make_shared<Node>(CONST, NodePtr(),
                                  NodePtr(), std::move(value));
}

NodePtr WpExpr::Node::CreateUndeterminedPredicate(llvm::Value *val) {
    auto varnode = CreateVar(val);
    auto upred =  std::make_shared<Node>(UPRED, std::move(varnode),
                                         NodePtr(), std::move(std::string("")));
    return upred;
}

void WpExpr::Node::fillUndeterminedPredicate(NodePtr &upred, const NodePtr &expr,
                                             const llvm::Value *val) {
    std::cout<<"upred before:"<<upred->ToString()<<"\n";
    std::cout<<"expr before:"<<expr->ToString()<<"\n";
    if (!upred)
    {
        return;
    }
    switch (upred->type) {
        case VAR:case CONST:
            break;
        case UPRED: {
            auto upredexpr = upred->left;
            upred = std::make_shared<Node>(*expr);
            substitute(upred, val, upredexpr);
            break;
        }
        case BINOP:
            fillUndeterminedPredicate(upred->right, expr, val);
            fillUndeterminedPredicate(upred->left, expr, val);
            break;
        case UNIOP:
            fillUndeterminedPredicate(upred->left, expr, val);
            break;
    }
    std::cout<<"upred after:"<<upred->ToString()<<"\n";
    std::cout<<"expr after:"<<expr->ToString()<<"\n";
}

NodePtr
WpExpr::Node::substitute(NodePtr &src, const llvm::Value *val, const NodePtr &expr) {
    if (!src)
    {
        return src;
    }
    switch (src->type) {
        case VAR:
            if (src->valueObj == val)//|| src->name == WpExpr::wp_init_var
            {
                src = expr;
            } else {
                //TODO: warning nothing substituted
            }
            break;
        case CONST:
            //TODO: warning nothing substituted
            break;
        case UPRED:
        case UNIOP:
            substitute(src->left, val, expr);
            break;
        case BINOP:
            substitute(src->left, val, expr);
            substitute(src->right, val, expr);
            break;
    }
    return src;
}

std::list<NodePtr> &
WpExpr::Node::substitute(std::list<NodePtr> &src, const llvm::Value *val, const NodePtr &expr) {
    if (src.empty())
    {
        return src;
    }
    for (auto item:src) {
        substitute(item, val, expr);
    }
    return src;
}

std::string WpExpr::Node::GetName() {
    if (this->type == VAR) {
        return this->valueObj->getName();
    } else {
        return "";
    }
    /*|| this->type == CONST) {

    }*/
}

void Node::fillUndeterminedPredicate(std::list<std::shared_ptr<Node> > &upred, const std::list<std::shared_ptr<Node>> &expr_list,
                                     const llvm::Value *val) {
    std::list<NodePtr> ret;
    for (auto expr:expr_list) {
        std::list<NodePtr> tmp;
        for (auto upred_elem:upred) {
            tmp.push_back(std::make_shared<Node>(*upred_elem));
        }
        fillUndeterminedPredicate(tmp, expr, val);

        ret.splice(ret.end(), tmp);
    }
    upred = ret;
}

void
Node::fillUndeterminedPredicate(std::list<std::shared_ptr<Node> > &upred, const std::shared_ptr<Node> &expr,
        const llvm::Value *val) {
    if (upred.empty()) {
        return;
    }
    std::cout << "before in overloaded filling:\n";
    std::cout << upred.front()->ToString() << "\n";
    for (auto &item:upred) {
        fillUndeterminedPredicate(item, expr, val);
    }
    std::cout << "after in overloaded filling:\n";
    std::cout << upred.front()->ToString() << "\n";
}


void Conjunction::Insert(NodePtr nodePtr) {
    std::string expr_str;
    expr_str = nodePtr->ToString();

    //auto x = visitedExprString.find();
    if (this->visitedExprString.find(expr_str) == this->visitedExprString.end()) {
        this->visitedExprString.insert(expr_str);
        this->conjunctionList.push_back(nodePtr);
    }
}
