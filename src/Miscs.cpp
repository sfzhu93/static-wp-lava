//
// Created by zsf on 19-3-19.
//

#include "Miscs.h"


//copied from higher version llvm

WpExpr::Operator predicate2WpExprOp(CmpInst::Predicate Pred) {//https://llvm.org/doxygen/classllvm_1_1CmpInst.html#a283f9a5d4d843d20c40bb4d3e364bb05
    //unordered sort contains NA
    switch (Pred) {
        case ICmpInst::ICMP_EQ:
            return WpExpr::EQ;
        case ICmpInst::ICMP_SGT:case ICmpInst::ICMP_UGT:
            return WpExpr::GT;
        case ICmpInst::ICMP_SGE:case ICmpInst::ICMP_UGE:
            return WpExpr::GE;
        case ICmpInst::ICMP_SLT:case ICmpInst::ICMP_ULT:
            return WpExpr::LT;
        case ICmpInst::ICMP_SLE:case ICmpInst::ICMP_ULE:
            return WpExpr::LE;
        case ICmpInst::ICMP_NE:
            return WpExpr::NEQ;
        case FCmpInst::FCMP_TRUE:
        case FCmpInst::FCMP_FALSE:
        default:
            assert(false);
/*
            return "unknown";
            return "false";
        case FCmpInst::FCMP_OEQ:
            return "oeq";
        case FCmpInst::FCMP_OGT:
            return "ogt";
        case FCmpInst::FCMP_OGE:
            return "oge";
        case FCmpInst::FCMP_OLT:
            return "olt";
        case FCmpInst::FCMP_OLE:
            return "ole";
        case FCmpInst::FCMP_ONE:
            return "one";
        case FCmpInst::FCMP_ORD:
            return "ord";
        case FCmpInst::FCMP_UNO:
            return "uno";
        case FCmpInst::FCMP_UEQ:
            return "ueq";
        case FCmpInst::FCMP_UGT:
            return "ugt";
        case FCmpInst::FCMP_UGE:
            return "uge";
        case FCmpInst::FCMP_ULT:
            return "ult";
        case FCmpInst::FCMP_ULE:
            return "ule";
        case FCmpInst::FCMP_UNE:
            return "une";
            return "true";*/
    }
}


const char *opcode2Name(unsigned opcode) {
    switch (opcode) {
        case Instruction::FAdd:case Instruction::Add:
            return "+";
        case Instruction::Sub:
            return "-";
        case Instruction::FMul:case Instruction::Mul:
            return "*";
        case Instruction::UDiv:
            return "/";
        case Instruction::SDiv:
            return "/";
        case Instruction::URem:
            return "mod";
        case Instruction::SRem:
            return "mod";
        default:
            return Instruction::getOpcodeName(opcode);
    }
}

const WpExpr::Operator opcode2WpExprOp(unsigned opcode) {
    switch (opcode) {
        case Instruction::FAdd:
        case Instruction::Add:
            return WpExpr::PLUS;
        case Instruction::Sub:
            return WpExpr::MINUS;
        case Instruction::FMul:
        case Instruction::Mul:
            return WpExpr::MUL;
        case Instruction::UDiv:
        case Instruction::SDiv:
            return WpExpr::DIV;
        case Instruction::URem:
        case Instruction::SRem:
            return WpExpr::MOD;
        default:
            std::cout << "Unrecognized opcode: " << opcode << "\n";
            //return Instruction::getOpcodeName(opcode);
    }
}


const char *WpExprOperator2CStr(WpExpr::Operator op) {
    switch (op) {
        case WpExpr::PLUS:
            return "+";
        case WpExpr::MINUS:
            return "-";
        case WpExpr::AND:
            return "and";
        case WpExpr::OR:
            return "or";
        case WpExpr::MUL:
            return "*";
        case WpExpr::DIV:
            return "/";
        case WpExpr::IND:
            return "[]";
        case WpExpr::MOD:
            return "%";
        case WpExpr::XOR:
            return "^";
        case WpExpr::TO_INT:
            return "TO_INT";
        case WpExpr::TO_REAL:
            return "TO_REAL";
        case WpExpr::NOT:
            return "not";
        case WpExpr::GT:
            return ">";
        case WpExpr::GE:
            return ">=";
        case WpExpr::EQ:
            return "=";
        case WpExpr::NEQ:
            return "!=";
        case WpExpr::LT:
            return "<";
        case WpExpr::LE:
            return "<=";
    }
    /*    enum Operator {PLUS, MINUS, AND, OR, MUL, DIV, IND, MOD, XOR,//binop
            TO_INT, TO_REAL, NOT, //Uniop
            GT, GE, EQ, NEQ, LT, LE //Compare
    };*/
}
