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
Node::NodePtr HandleConstOrVar(Value *value) {
    Node::NodePtr ret;
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
        ret = Node::CreateVar(value->getName());//TODO: would move fuck up here?
    }
    return ret;
}

/**
 *
 * @param inst The GetElementPtr instruction.
 * @param expr The reference to the smart pointer of expression to be substituted.
 */
void handleGetElementPtr(GetElementPtrInst &inst, Node::NodePtr &expr) {
    auto a = inst.getOperand(0);
    Node::NodePtr value;
    if (isa<GlobalVariable>(a) && a->getName()=="lava_val") {
        value = Node::CreateBinOp(
                Node::CreateVar(std::string("lava_val")),
                Node::CreateVar(inst.getOperand(2)->getName()),
                "[]"
        );
    }else{
        value = Node::CreateVar("_");
    }
    /*for (auto i = GEPIns.op_begin();i!=GEPIns.op_end();++i)
    {
        if (auto )
    }*/

    Node::substitute(expr, inst.getName(), value);
}

/**
 *
 * @param inst The Load instruction.
 * @param expr The reference to the smart pointer of expression to be substituted.
 */
void handleLoad(LoadInst &inst, Node::NodePtr &expr){
    Node::substitute(expr, inst.getName(), Node::CreateVar(inst.getOperand(0)->getName()));

    //TODO: add more notations for the address
}


void handleRet(ReturnInst &inst, Node::NodePtr &expr) {
    if (inst.getReturnValue())
    {
        expr = Node::CreateUndeterminedPredicate(inst.getReturnValue()->getName());
        /*Node::substitute(expr,
                         std::string("call"),//TODO: replace __ret_val__
                         Node::CreateVar(inst.getReturnValue()->getName()));*/

    }
}

void handleBranch(BranchInst &inst, Node::NodePtr &expr) {
    auto cnt = inst.getNumSuccessors();
    if (cnt>1)//conditional
    {
        auto cond = inst.getOperand(0)->getName();
        auto br1 = inst.getSuccessor(0)->getName();
        auto br2 = inst.getSuccessor(1)->getName();
        Node::substitute(expr, br1, Node::CreateVar(cond));
        Node::substitute(expr, br2, Node::CreateUniOp(Node::CreateVar(cond), "NOT"));
    }else { //unconditional. TODO: replace dest name with basic block's name

    }
}


void handleSelect(SelectInst &inst, Node::NodePtr &expr) {
    auto cond = inst.getOperand(0)->getName();
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto aug2 = HandleConstOrVar(inst.getOperand(2));
    auto lhs = inst.getName();
    auto new_wp_left = std::make_shared<Node>(*expr);
    auto new_wp_right = std::make_shared<Node>(*expr);
    Node::substitute(new_wp_left, lhs, aug1);// WP-> WP[aug1/lhs]
    Node::substitute(new_wp_right, lhs, aug2);//WP -> WP[aug2/lhs]
    new_wp_left = Node::CreateBinOp(Node::CreateVar(cond),
                                    std::move(new_wp_left),
                                    std::string(strAnd));
    new_wp_right = Node::CreateBinOp(Node::CreateUniOp(
            Node::CreateVar(cond),
            std::string(strNot)),
                                     std::move(new_wp_right),
                                     std::string(strAnd));
    auto new_wp = Node::CreateBinOp(std::move(new_wp_left),
                                    std::move(new_wp_right),
                                    std::string(strOr));
    expr = std::move(new_wp);
}


void handleBinaryOperator(BinaryOperator &inst, Node::NodePtr &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto op = std::string(opcode2Name(inst.getOpcode()));
    auto lhs = inst.getName();
    Node::substitute(expr, lhs,
                     Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                       std::move(op)));
}


void handleICmp(ICmpInst &inst, Node::NodePtr &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto predicate = getPredicateName(inst.getUnsignedPredicate());
    auto cond = Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                  std::string(predicate));
    auto lhs = inst.getName();
    Node::substitute(expr, lhs, cond);
}

void handlePHI(PHINode &inst, Node::NodePtr &expr) {
    auto lhs = inst.getName();
    auto cnt = inst.getNumIncomingValues();
    if (cnt == 1) {
        outs() << "This is not supposed to happen.\n";//TODO
    } else {
        //first handling the first two incoming values
        auto cond1 = inst.getIncomingBlock(0)->getName();
        auto aug1 = HandleConstOrVar(inst.getOperand(0));
        auto new_wp = std::make_shared<Node>(*expr);
        Node::NodePtr new_wp_right = std::make_shared<Node>();
        Node::substitute(new_wp, lhs, aug1);// WP-> WP[aug1/lhs]
        new_wp = Node::CreateBinOp(Node::CreateVar(cond1),
                                   std::move(new_wp),
                                   std::string(strAnd));
        for (unsigned i = 1; i < cnt; ++i) {//TODO: design cases to cover different cnt
            auto cond = inst.getIncomingBlock(i)->getName();
            auto aug = HandleConstOrVar(inst.getOperand(1));
            new_wp_right = std::make_shared<Node>(*expr);
            Node::substitute(new_wp_right, lhs, aug);
            new_wp_right = Node::CreateBinOp(Node::CreateVar(cond),
                                             std::move(new_wp_right),
                                             std::string(strAnd));
            new_wp = Node::CreateBinOp(std::move(new_wp),
                                       std::move(new_wp_right),
                                       std::string(strOr));
        }
        expr = std::move(new_wp);
        outs() <<"end of handlPHI\n";
    }
}


void handleCast(CastInst &inst, Node::NodePtr &expr) {
    auto lhs = inst.getName();
    switch (inst.getOpcode()){
        case Instruction::FPToUI:
        case Instruction::FPToSI:{
            auto aug = HandleConstOrVar(inst.getOperand(0));
            auto op = std::string("to_int");
            Node::substitute(expr, lhs,
                             Node::CreateUniOp(std::move(aug),
                                               std::move(op)));
            break;
        }
        case Instruction::UIToFP:
        case Instruction::SIToFP: {
            auto aug = HandleConstOrVar(inst.getOperand(0));
            auto op = std::string("to_real");
            Node::substitute(expr, lhs,
                             Node::CreateUniOp(std::move(aug),
                                               std::move(op)));
            break;
        }

    }
}


void handleLogicOp(BinaryOperator &inst, Node::NodePtr &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    const char * op;
    switch (inst.getOpcode())
    {
        case Instruction::And:op = "and";
            break;
        case Instruction::Or:op = "or";
            break;
        case Instruction::Xor:op = "xor";
            break;
        default:
            assert(false);
    }
    auto lhs = inst.getName();
    Node::substitute(expr, lhs,
                     Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                       op));
    //TODO: not implemented
}


void handleFCmp(FCmpInst &inst, Node::NodePtr &expr) {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto predicate = getPredicateName(inst.getPredicate());
    auto cond = Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                  std::string(predicate));
    auto lhs = inst.getName();
    Node::substitute(expr, lhs, cond);
}
