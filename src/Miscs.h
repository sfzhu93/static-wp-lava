//
// Created by zsf on 19-3-19.
//

#ifndef STATIC_WP_LAVA_MISCS_H
#define STATIC_WP_LAVA_MISCS_H
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "WpExpr.h"
#include <unordered_map>

using namespace llvm;


StringRef getPredicateName(CmpInst::Predicate Pred);
const char *opcode2Name(unsigned opcode);
const WpExpr::Operator opcode2WpExprOp(unsigned opcode);
WpExpr::Operator predicate2WpExprOp(CmpInst::Predicate Pred);
const char * WpExprOperator2CStr(WpExpr::Operator op);

template <class T, class PtrType>
bool mapContainsKey(std::unordered_map<std::uintptr_t, T> &map, PtrType ptr) {
    auto p = reinterpret_cast<std::uintptr_t>(ptr);
    return !(map.find(p) == map.end());
}

#endif //STATIC_WP_LAVA_MISCS_H
