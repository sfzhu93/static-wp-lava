#include <iostream>
#include "WpExpr.h"

using namespace WpExpr;

Node::NodePtr test1()
{
    auto expr = Node::CreateBinOp(Node::CreateConst("1"), Node::CreateVar("a"), std::string("+"));
    Node::NodePtr expr_0 = std::make_shared<Node>(*expr);
    Node::substitute(expr, std::string("a"), Node::CreateConst("1"));
    Node::NodePtr con = Node::CreateBinOp(std::move(expr_0), std::move(expr), std::string("+"));
    std::cout<<con->ToString()<<std::endl;
    return con;
}

int main()
{

    std::cout<<test1()->ToString()<<std::endl;
    return 0;
}
