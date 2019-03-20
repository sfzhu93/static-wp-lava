//
// Created by zsf on 19-3-19.
//

#ifndef STATIC_WP_LAVA_MISCS_H
#define STATIC_WP_LAVA_MISCS_H
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
using namespace llvm;


StringRef getPredicateName(CmpInst::Predicate Pred);
const char *opcode2Name(unsigned opcode);



#endif //STATIC_WP_LAVA_MISCS_H
