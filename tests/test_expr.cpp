#include "WpExpr.h"
#include <iostream>

using namespace WpExpr;

Node::NodePtr test1() {
  auto expr = CreateBinOp(CreateConst("1"), CreateVar("a"),
                                Operator::PLUS);
                                // std::string("+"));
  Node::NodePtr expr_0 = std::make_shared<Node>(*expr);
  // Node::substitute(expr, std::string("a"), CreateConst("1"));
  Node::NodePtr con =
      CreateBinOp(std::move(expr_0), std::move(expr),
                                Operator::PLUS);
  std::cout << con->ToString() << std::endl;
  return con;
}

int main() {

  std::cout << test1()->ToString() << std::endl; // should be ((1 + a) + (1 +
                                                 // 1))
  return 0;
}
