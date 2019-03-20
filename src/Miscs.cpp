//
// Created by zsf on 19-3-19.
//

#include "Miscs.h"


//copied from higher version llvm
StringRef getPredicateName(CmpInst::Predicate Pred) {
    switch (Pred) {
        default:
            return "unknown";
        case FCmpInst::FCMP_FALSE:
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
        case FCmpInst::FCMP_TRUE:
            return "true";
        case ICmpInst::ICMP_EQ:
            return "=";
        case ICmpInst::ICMP_NE:
            return "ne";
        case ICmpInst::ICMP_SGT:
            return "sgt";
        case ICmpInst::ICMP_SGE:
            return "sge";
        case ICmpInst::ICMP_SLT:
            return "slt";
        case ICmpInst::ICMP_SLE:
            return "sle";
        case ICmpInst::ICMP_UGT:
            return ">";
        case ICmpInst::ICMP_UGE:
            return "uge";
        case ICmpInst::ICMP_ULT:
            return "ult";
        case ICmpInst::ICMP_ULE:
            return "ule";
    }
}


const char *opcode2Name(unsigned opcode) {
    switch (opcode) {
        case Instruction::Sub:
            return "-";
        case Instruction::Mul:
            return "*";
        case Instruction::UDiv:
            return "/";
        case Instruction::SDiv:
            return "/";
        case Instruction::URem:
            return "mod";
        case Instruction::SRem:
            return "mod";
        case Instruction::Add:
            return "+";
        default:
            return Instruction::getOpcodeName(opcode);
    }
}