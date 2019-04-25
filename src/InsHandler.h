//
// Created by zsf on 19-3-1.
//

#ifndef STATIC_WP_LAVA_INSHANDLER_H
#define STATIC_WP_LAVA_INSHANDLER_H

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "WpExpr.h"
#include <list>

using namespace llvm;
using namespace WpExpr;

NodePtr HandleConstOrVar(Value *value);

void handleGetElementPtr(GetElementPtrInst &inst, std::list<NodePtr> &expr_list);
void handleLoad(LoadInst &inst, std::list<NodePtr> &expr_list);
void handleRet(ReturnInst &inst, std::list<NodePtr> &expr_list);
void handleBranch(BranchInst &inst, std::list<NodePtr> &expr_list);
void handleSelect(SelectInst &inst, std::list<NodePtr> &expr_list);
void handleBinaryOperator(BinaryOperator &inst, std::list<NodePtr> &expr_list);
void handlePHI(PHINode &inst, std::list<NodePtr> &expr_list);
void handleICmp(ICmpInst &inst, std::list<NodePtr> &expr_list);
void handleCast(CastInst &inst, std::list<NodePtr> &expr_list);
void handleLogicOp(BinaryOperator &inst, std::list<NodePtr> &expr_list);
void handleFCmp(FCmpInst &inst, std::list<NodePtr> &expr_list);

#endif //STATIC_WP_LAVA_INSHANDLER_H
