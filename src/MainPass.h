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
#include <list>
#include <queue>
#include <tuple>


using namespace llvm;
using namespace WpExpr;

namespace {

    typedef enum FuncVisitState {
        False,
        Pure,
        NotPure
    };

    //ModulePass for interprocedural analysis
    struct MainPass : public ModulePass {
        static char ID;
        std::string modulename;
        MainPass() : ModulePass(ID) {}
        NodePtr WP;
        std::list<NodePtr> WPConstraintSet;
        bool InWP = false;

        std::unordered_map<std::uintptr_t, int> naming_map;
        std::unordered_map<std::uintptr_t, FuncVisitState > visitedFunc;
        std::unordered_map<std::uintptr_t, std::list<NodePtr> > PureFuncConstaintSet;
        WpPrinter wpPrinter;

        int naming_index = 0;

        void getAnalysisUsage(AnalysisUsage &AU) const override;

        std::string getName(Value *value);

        void printOperandNames(Instruction &ins);

        std::string buildLavaVarName(int64_t bug_no);

        NodePtr handleGEP(GetElementPtrInst &GEPIns);

        Instruction* findWpEnd(Function& function);

        NodePtr setWp(Instruction *ins);

        void copyToConstraintList(const std::list<NodePtr> &a, std::list<NodePtr> &target);

        std::list<NodePtr> newHandleFunctionCall(Function& function, bool started);

        void printConstraints(const std::list<NodePtr> &constraint_list, const Instruction &instruction);

        void EliminateUnsatConstraints(std::list<NodePtr> &constraint_list) const;

        bool runOnModule(Module &M) override;

        void handleInstructions(std::list<NodePtr> &constraint_list, Instruction &instruction, bool &isPureFunc,
                            std::string FuncName);
    };
}

#endif