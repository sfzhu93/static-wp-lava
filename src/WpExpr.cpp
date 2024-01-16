//
// Created by suz305 on 2/27/19.
//

#include "WpExpr.h"
#include "Miscs.h"

namespace WpExpr {

Node::Node(const Node &node) : Node() {
  this->type = node.type;
  this->left =
      node.left != nullptr ? std::make_shared<Node>(*node.left) : nullptr;
  this->right =
      node.right != nullptr ? std::make_shared<Node>(*node.right) : nullptr;
  this->value = node.value;
  this->valueObj = node.valueObj;
  this->operatorType = node.operatorType;
}

std::string Node::ToString() {
  std::string ret;
  switch (this->type) {
  case UNIOP:
    ret = std::string("(") + WpExprOperator2CStr(this->operatorType) + " " +
          this->left->ToString() + ")";
    break;
  case BINOP:
    ret = "(" + this->left->ToString() + " " +
          WpExprOperator2CStr(this->operatorType) + " " +
          this->right->ToString() + ")";
    break;
  case CONST:
    return this->name;
  case VAR: {
    if (this->valueObj) {
      // std::string tmp_name = ;
      return this->valueObj->getName().str();
    } else {
      return this->value;
    }
  }
  case UPRED:
    return "UPred(" + this->left->ToString() + ")";
  }
  return ret;
}

std::string Node::ToSMTLanguage() {
  std::string ret;
  switch (this->type) {
  case UNIOP:
    ret = std::string("(") + WpExprOperator2CStr(this->operatorType) + " " +
          this->left->ToSMTLanguage() + ")";
    break;
  case BINOP:
    ret = std::string("( ") + WpExprOperator2CStr(this->operatorType) + " " +
          this->left->ToSMTLanguage() + " " + this->right->ToSMTLanguage() +
          ")";
    break;
  case CONST:
  case VAR:
    return this->ToString();
  case UPRED:
    return "UPred(" + this->left->ToSMTLanguage() + ")";
  }
  return ret;
}

Node::NodePtr CreateBinOp(Node::NodePtr &&left, Node::NodePtr &&right,
                          Operator optr) {
  auto ret =
      std::make_shared<Node>(BINOP, std::move(left), std::move(right), "");
  ret->operatorType = optr;
  return ret;
}

Node::NodePtr CreateUniOp(Node::NodePtr &&left, Operator optr) {
  auto ret =
      std::make_shared<Node>(UNIOP, std::move(left), Node::NodePtr(), "");
  ret->operatorType = optr;
  return ret;
}

Node::NodePtr CreateVar(llvm::Value *val) {
  auto name = val->getName().str();
  // std::cout << "CreateVar: has_name = " << val->hasName() << " name = " << name << "\n";
  auto ret =
      std::make_shared<Node>(VAR, Node::NodePtr(), Node::NodePtr(), name);
  ret->valueObj = val;
  return ret;
}

Node::NodePtr CreateVar(const std::string &name) {
  auto ret =
      std::make_shared<Node>(VAR, Node::NodePtr(), Node::NodePtr(), name);
  ret->valueObj = nullptr;
  return ret;
}

Node::NodePtr CreateConst(const std::string &value) {
  return std::make_shared<Node>(CONST, Node::NodePtr(), Node::NodePtr(),
                                value);
}

Node::NodePtr CreateUndeterminedPredicate(llvm::Value *val) {
  auto varnode = CreateVar(val);
  auto upred = std::make_shared<Node>(
      UPRED, std::move(varnode), Node::NodePtr(), "");
  return upred;
}

void Node::fillUndeterminedPredicate(Node::NodePtr &upred,
                                     const Node::NodePtr &expr,
                                     const llvm::Value *val) {
  if (!upred) {
    return;
  }
  switch (upred->type) {
  case VAR:
  case CONST:
    break;
  case UPRED: {
    auto upredexpr = upred->left;
    upred = std::make_shared<Node>(*expr);
    Substitute(upred, val, upredexpr);
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

Node::NodePtr Substitute(Node::NodePtr &src, const llvm::Value *val,
                         const Node::NodePtr &expr) {
  if (!src) {
    return src;
  }
  switch (src->type) {
  case VAR:
    if (src->valueObj == val) //|| src->name == wp_init_var
    {
      src = expr;
    } else {
      // TODO: warning nothing substituted
    }
    break;
  case CONST:
    // TODO: warning nothing substituted
    break;
  case UPRED:
  case UNIOP:
    Substitute(src->left, val, expr);
    break;
  case BINOP:
    Substitute(src->left, val, expr);
    Substitute(src->right, val, expr);
    break;
  }
  return src;
}

std::string Node::GetName() {
  if (this->type == VAR) {
    return this->valueObj->getName().str();
  } else {
    return "";
  }
  /*|| this->type == CONST) {

  }*/
}

} // namespace WpExpr