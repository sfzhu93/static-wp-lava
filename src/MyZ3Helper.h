//
// Created by zsf on 19-4-15.
//

#ifndef STATIC_WP_LAVA_MYZ3_H
#define STATIC_WP_LAVA_MYZ3_H

#include "z3++.h"
#include <unordered_map>
#include "WpExpr.h"

using namespace z3;

inline expr to_int(expr const & a) { Z3_ast r = Z3_mk_real2int(a.ctx(), a); a.check_error(); return expr(a.ctx(), r); }
inline expr Xor(expr const & a, expr const &b) { Z3_ast r = Z3_mk_xor(a.ctx(), a, b); a.check_error(); return expr(a.ctx(), r); }

class SolverContext {
public:
    context Context;
    std::unordered_map<std::string, Z3_ast > Env;

    SolverContext(): Context(), Env() {
    }

    z3::expr WpExprToZ3Expr(const WpExpr::Node::NodePtr &node) {
        //z3::expr ret = c.int_const("__unused__value__");
        switch (node->type) {
            case WpExpr::UNIOP:
                switch (node->operatorType) {
                    case WpExpr::NOT:
                        return !WpExprToZ3Expr(node->left);
                    case WpExpr::TO_INT:
                        return to_int(WpExprToZ3Expr(node->left));
                    case WpExpr::TO_REAL:
                        return to_real(WpExprToZ3Expr(node->left));
                    default:
                        std::cout<<"unexpected unary operator type.\n";
                        break;
                }
                break;
            case WpExpr::BINOP:
                switch (node->operatorType) {
                    case WpExpr::PLUS:
                        return WpExprToZ3Expr(node->left) + WpExprToZ3Expr(node->right);
                    case WpExpr::MINUS:
                        return WpExprToZ3Expr(node->left) - WpExprToZ3Expr(node->right);
                    case WpExpr::AND:
                        return WpExprToZ3Expr(node->left) && WpExprToZ3Expr(node->right);
                    case WpExpr::OR:
                        return WpExprToZ3Expr(node->left) || WpExprToZ3Expr(node->right);
                    case WpExpr::MUL:
                        return WpExprToZ3Expr(node->left) * WpExprToZ3Expr(node->right);
                    case WpExpr::DIV:
                        return WpExprToZ3Expr(node->left) / WpExprToZ3Expr(node->right);
                        /*case WpExpr::IND:
                            return this->left->ToZ3Expr(c) && this->right->ToZ3Expr(c);*/
                        /*case WpExpr::MOD:
                            return this->left->ToZ3Expr(c) % this->right->ToZ3Expr(c);*/
                        //Z3_mk_mod and Z3_mk_rem
                    case WpExpr::XOR:
                        return Xor(WpExprToZ3Expr(node->left), WpExprToZ3Expr(node->right));
                    case WpExpr::GT:
                        return WpExprToZ3Expr(node->left) > WpExprToZ3Expr(node->right);
                    case WpExpr::GE:
                        return WpExprToZ3Expr(node->left) >= WpExprToZ3Expr(node->right);
                    case WpExpr::EQ:
                        return WpExprToZ3Expr(node->left) == WpExprToZ3Expr(node->right);
                    case WpExpr::NEQ:
                        return WpExprToZ3Expr(node->left) != WpExprToZ3Expr(node->right);
                    case WpExpr::LT:
                        return WpExprToZ3Expr(node->left) < WpExprToZ3Expr(node->right);
                    case WpExpr::LE:
                        return WpExprToZ3Expr(node->left) <= WpExprToZ3Expr(node->right);
                    default:
                        std::cout<<"unexpected binary operator type.\n";
                        break;
                }
                break;
            case WpExpr::CONST:
                return this->Context.int_val(node->value.c_str());
            case WpExpr::VAR: {
                auto name = node->GetName();
                if (this->Env.find(name) == Env.end()) {
                    auto con = this->Context.int_const(name.c_str());
                    this->Env[name] = con;
                }
                return expr(this->Context, this->Env[name]);
            }

            case WpExpr::UPRED:
                return this->Context.bool_const("upred");//"UPred("+ this->left->ToSMTLanguage() + ")";
        }
    }
};

#endif //STATIC_WP_LAVA_MYZ3_H
