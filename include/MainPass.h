//
// Created by suz305 on 2/26/19.
//

#ifndef STATIC_WP_LAVA_MAINPASS_H
#define STATIC_WP_LAVA_MAINPASS_H
#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/ADT/PostOrderIterator.h"

#include "WpExpr.h"
#include "Miscs.h"
#include "InsHandler.h"
#include "WpPrinter.h"
#include <iostream>
//#include <z3++.h>
#include <unordered_map>
#include "MyZ3Helper.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace {
class MainPass : public llvm::PassInfoMixin<MainPass> {
public:
    llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &AM);
    std::string modulename;
    WpExpr::Node::NodePtr WP;
    bool InWP = false;
    std::unordered_map<std::uintptr_t, int> naming_map;
    std::unordered_map<std::uintptr_t, int> visitedFunc;
    std::unordered_map<std::uintptr_t, bool > isPureFunc;
    WpPrinter wpPrinter;

    int naming_index = 0;

    std::string getName(Value *value);
    void printOperandNames(Instruction &ins);
    std::string buildLavaVarName(int64_t bug_no);
    Node::NodePtr handleGEP(GetElementPtrInst &GEPIns);
    Node::NodePtr handleFunctionCall(Function& function);
};

}

#endif //STATIC_WP_LAVA_MAINPASS_H
