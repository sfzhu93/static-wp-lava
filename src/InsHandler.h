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

using namespace llvm;
using namespace WpExpr;

Node::NodePtr HandleConstOrVar(Value *value);

void handleGetElementPtr(GetElementPtrInst &inst, Node::NodePtr& expr);
void handleLoad(LoadInst &inst, Node::NodePtr &expr);
void handleRet(ReturnInst &inst, Node::NodePtr &expr);
void handleBranch(BranchInst &inst, Node::NodePtr &expr);
void handleSelect(SelectInst &inst, Node::NodePtr &expr);
void handleBinaryOperator(BinaryOperator &inst, Node::NodePtr &expr);
void handlePHI(PHINode &inst, Node::NodePtr &expr);
void handleICmp(ICmpInst &inst, Node::NodePtr &expr);
void handleCast(CastInst &inst, Node::NodePtr &expr);

#endif //STATIC_WP_LAVA_INSHANDLER_H
