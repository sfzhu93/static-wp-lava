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

WpExpr::Node::NodePtr
WpExpr::Node::CreateBinOp(WpExpr::Node::NodePtr &&left, WpExpr::Node::NodePtr &&right, WpExpr::Operator optr) {
    auto ret = std::make_shared<Node>(BINOP, std::move(left), std::move(right), "");
    ret->operatorType = optr;
    return ret;
}

WpExpr::Node::NodePtr WpExpr::Node::CreateUniOp(WpExpr::Node::NodePtr &&left, WpExpr::Operator optr) {
    auto ret = std::make_shared<Node>(UNIOP, std::move(left), NodePtr(), "");
    ret->operatorType = optr;
    return ret;
}

WpExpr::Node::NodePtr WpExpr::Node::CreateVar(llvm::Value *val) {
    auto name = val->getName();
    auto ret = std::make_shared<Node>(VAR, NodePtr(),
                                      NodePtr(), name);
    ret->valueObj = val;
    return ret;
}

WpExpr::Node::NodePtr WpExpr::Node::CreateVar(std::string name) {
    auto ret = std::make_shared<Node>(VAR, NodePtr(),
                                      NodePtr(), std::move(name));
    ret->valueObj = nullptr;
    return ret;
}

WpExpr::Node::NodePtr WpExpr::Node::CreateConst(std::string value) {
    return std::make_shared<Node>(CONST, NodePtr(),
                                  NodePtr(), std::move(value));
}

WpExpr::Node::NodePtr WpExpr::Node::CreateUndeterminedPredicate(llvm::Value *val) {
    auto varnode = CreateVar(val);
    auto upred =  std::make_shared<Node>(UPRED, std::move(varnode),
                                         NodePtr(), std::move(std::string("")));
    return upred;
}

void WpExpr::Node::fillUndeterminedPredicate(WpExpr::Node::NodePtr &upred, const WpExpr::Node::NodePtr &expr,
                                             const llvm::Value *val) {
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
}

WpExpr::Node::NodePtr
WpExpr::Node::substitute(WpExpr::Node::NodePtr &src, const llvm::Value *val, const WpExpr::Node::NodePtr &expr) {
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

std::string WpExpr::Node::GetName() {
    if (this->type == VAR) {
        return this->valueObj->getName();
    } else {
        return "";
    }
    /*|| this->type == CONST) {

    }*/
}
