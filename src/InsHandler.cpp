//
// Created by zsf on 19-3-1.
//

#include "InsHandler.h"
#include "Miscs.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"

using namespace llvm;
using namespace WpExpr;

static const char * strAnd = "and";
static const char * strOr = "or";
static const char * strNot = "not";


/**
 *
 * @param value The value should be either a Constant or an Instruction.
 * @return If the value is an Instruction, it returns the left-hand side value
 * (i.e. the name of the Instruction). Otherwise it returns the literal value
 * of the constant.
 */
NodePtr HandleConstOrVar(Value *value) {
    NodePtr ret;
    if(auto c = dyn_cast<Constant>(value))
    {
        auto type = c->getType()->getTypeID();
        switch (type) {
            case Type::IntegerTyID:
                ret = Node::CreateConst(
                        std::to_string(c->getUniqueInteger().getSExtValue()));//TODO: may change to ZExt
                break;
            case Type::HalfTyID:
            case Type::FloatTyID:
            case Type::DoubleTyID:
            case Type::X86_FP80TyID:
            case Type::FP128TyID:
            case Type::PPC_FP128TyID: {
                auto value = dyn_cast<ConstantFP>(c);

                ret = Node::CreateConst(std::to_string(value->getValueAPF().convertToDouble()));
                break;
            }
            default:
                outs()<<"unresolved constant type!\n";
        }
    }else//return the variable name
    {
        ret = Node::CreateVar(value);//TODO: check if move works well here
    }
    return ret;
}

/**
 *
 * @param inst The GetElementPtr instruction.
 * @param expr_list The reference to the smart pointer of expression to be substituted.
 */
void handleGetElementPtr(GetElementPtrInst &inst, std::list<NodePtr> &expr_list) {
    auto a = inst.getOperand(0);
    auto nops = inst.getNumOperands();
    for (int i=0;i<nops;++i){
        outs()<<"GEPOperand: "<<inst.getOperand(i)->getName()<<"\n";
    }
    NodePtr value;
    if (isa<GlobalVariable>(a) && a->getName()=="lava_val") {
        value = Node::CreateBinOp(
                Node::CreateVar(std::string("lava_val")),
                Node::CreateVar(inst.getOperand(2)->getName()),
                WpExpr::Operator::IND
        );
    }else{
        value = Node::CreateVar("_");
    }
    /*for (auto i = GEPIns.op_begin();i!=GEPIns.op_end();++i)
    {
        if (auto )
    }*/

    Node::substitute(expr_list, &inst, value);
}

/**
 *
 * @param inst The Load instruction.
 * @param expr_list The reference to the smart pointer of expression to be substituted.
 */
void handleLoad(LoadInst &inst, std::list<NodePtr> &expr_list){
    Node::substitute(expr_list, &inst, Node::CreateVar(inst.getOperand(0)));

    //TODO: add more notations for the address
}


void handleRet(ReturnInst &inst, std::list<NodePtr> &expr_list) {
    assert(expr_list.empty());
    NodePtr expr;
    if (inst.getReturnValue())
    {
        expr = Node::CreateUndeterminedPredicate(inst.getReturnValue());
        expr_list.push_front(expr);
        /*Node::substitute(expr,
                         std::string("call"),//TODO: replace __ret_val__
                         Node::CreateVar(inst.getReturnValue()->getName()));*/

    } /*else
        expr = NodePtr();*/
}

void handleBranch(BranchInst &inst, std::list<NodePtr> &expr_list) {
    auto cnt = inst.getNumSuccessors();
    if (cnt>1)//conditional
    {
        auto cond = inst.getOperand(0);
        outs()<<inst.getSuccessor(0)->getName()<<"\n";
        outs()<<inst.getSuccessor(1)->getName()<<"\n";
        Node::substitute(expr_list, inst.getSuccessor(0), Node::CreateVar(cond));
        Node::substitute(expr_list, inst.getSuccessor(1), Node::CreateUniOp(Node::CreateVar(cond), WpExpr::NOT));
    }else { //unconditional. TODO: replace dest name with basic block's name

    }
}

void handleSelect(SelectInst &inst, std::list<NodePtr> &expr_list) {
    auto cond = inst.getOperand(0);
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto aug2 = HandleConstOrVar(inst.getOperand(2));
    std::list<NodePtr> tmp_list;
    for (auto i = expr_list.begin();i!=expr_list.end();++i){//(auto expr:expr_list) {
        auto &expr = *i;
        auto expr_neg = std::make_shared<Node>(*expr);
        Node::substitute(expr, &inst, aug1);
        expr = Node::CreateBinOp(Node::CreateVar(cond),
                std::move(expr),
                WpExpr::AND);
        expr_neg = Node::CreateBinOp(Node::CreateUniOp(Node::CreateVar(cond), WpExpr::NOT),
                std::move(expr_neg),
                WpExpr::AND);
        Node::substitute(expr_neg, &inst, aug2);
        tmp_list.push_back(expr_neg);
    }
    expr_list.splice(expr_list.end(), tmp_list);
}


void handleBinaryOperator(BinaryOperator &inst, std::list<NodePtr> &expr_list) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto op = opcode2WpExprOp(inst.getOpcode());//std::string((inst.getOpcode()));
    Node::substitute(expr_list, &inst, Node::CreateBinOp(std::move(aug0), std::move(aug1), op));
}


void handleICmp(ICmpInst &inst, std::list<NodePtr> &expr_list) {
    auto llvmpred = inst.getUnsignedPredicate();
    NodePtr cond;
/*    if (llvmpred == FCmpInst::FCMP_FALSE) {
        cond = Node::CreateConst("false");
    } else if (llvmpred == FCmpInst::FCMP_TRUE) {
        cond = Node::CreateConst("true");
    } else {*/
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto predicate = predicate2WpExprOp(llvmpred);
    cond = Node::CreateBinOp(std::move(aug0), std::move(aug1), predicate);
//    }
    Node::substitute(expr_list, &inst, cond);
}

void handlePHI(PHINode &inst, std::list<NodePtr> &expr_list) {
    auto lhs = inst.getName();
    auto cnt = inst.getNumIncomingValues();
    if (cnt == 1) {
        outs() << "This is not supposed to happen.\n";//TODO
    } else {
        //first handling the first two incoming values
        auto cond1 = inst.getIncomingBlock(0);
        auto aug1 = HandleConstOrVar(inst.getOperand(0));
        std::list<NodePtr> tmp_list;
        for (auto i = 0;i<cnt;++i) {
            auto cond = inst.getIncomingBlock(i);
            auto aug = HandleConstOrVar(inst.getOperand(i));
            for (auto expr:expr_list) {
                auto new_wp = std::make_shared<Node>(*expr);
                Node::substitute(new_wp, &inst, aug);
                new_wp = Node::CreateBinOp(Node::CreateVar(cond),
                                           std::move(new_wp),
                                           WpExpr::AND);
                tmp_list.push_back(new_wp);
            }
        }
        expr_list = tmp_list;
        outs() <<"end of handlPHI\n";
    }
}


void handleCast(CastInst &inst, std::list<NodePtr> &expr) {
    auto lhs = inst.getName();
    switch (inst.getOpcode()) {
        case Instruction::FPToUI:
        case Instruction::FPToSI: {
            auto aug = HandleConstOrVar(inst.getOperand(0));
            auto op = std::string("to_int");
            Node::substitute(expr, &inst,
                             Node::CreateUniOp(std::move(aug),
                                               WpExpr::TO_INT));
            break;
        }
        case Instruction::UIToFP:
        case Instruction::SIToFP: {
            auto aug = HandleConstOrVar(inst.getOperand(0));
            auto op = std::string("to_real");
            Node::substitute(expr, &inst,
                             Node::CreateUniOp(std::move(aug),
                                               WpExpr::TO_REAL));
            break;
        }
    }
}


void handleLogicOp(BinaryOperator &inst, std::list<NodePtr> &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    WpExpr::Operator op;
    switch (inst.getOpcode())
    {
        case Instruction::And:op = WpExpr::AND;
            break;
        case Instruction::Or:op = WpExpr::OR;
            break;
        case Instruction::Xor:op = WpExpr::XOR;
            break;
        default:
            assert(false);
    }
    auto lhs = inst.getName();
    Node::substitute(expr, &inst,
                     Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                       op));
}


void handleFCmp(FCmpInst &inst, std::list<NodePtr> &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto predicate = predicate2WpExprOp(inst.getPredicate());
    auto cond = Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                  predicate);
    auto lhs = inst.getName();
    Node::substitute(expr, &inst, cond);
}
