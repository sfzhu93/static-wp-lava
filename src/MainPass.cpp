//
// Created by suz305 on 2/26/19.
//

#include "MainPass.h"

#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/IR/CFG.h"
//#include <z3++.h>


using namespace llvm;

namespace {
    struct MainPass : public FunctionPass {
        static char ID;
        MainPass() : FunctionPass(ID) {}

        /*void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }*/

        bool runOnFunction(Function &F) override {
            /*
             * We Identify the SCCs of the BB graph, then topological sort these SCCs.
             * Loop usually collapse into a single SCC.
             */
            outs()<<"In Function:"<<F.getName()<<"()\n";
            //if(F.getName().compare("computeRecordScores")!=0)
            //  return false;

            /*
             * Iterate over Single Connected Component Basic Blocks(SCCs Basic Blocks)
             */
            for(scc_iterator<Function *> BB=scc_begin(&F),BBE=scc_end(&F); BB!=BBE;++BB){
                //Since each SCC-BB can have multiple Basic Blocks
                const std::vector<BasicBlock *> &SCCBBs=*BB;
                //Iterate over SCCs Basic Block and in each BB traverse Instruction in reverse
                for(std::vector<BasicBlock*>::const_iterator BBI = SCCBBs.begin();BBI!=SCCBBs.end();++BBI){
                    outs()<<"In BB:"<<(*BBI)->getName()<<"\n";
                    for(BasicBlock::reverse_iterator Ins=(*BBI)->rbegin();Ins!=(*BBI)->rend();++Ins){
                        Instruction &instruction = *Ins;
                        instruction.print(outs());
                        outs()<<"\n";
                        //instruction.dump();
                    }
                }
            }
            return true;
        }
    };
}

char MainPass::ID = 0;
//For opt

static RegisterPass<MainPass> X("wpgen","Weakest Precondition Analysis");




// Automatically enable the pass for Clang .
// http://adriansampson.net/blog/clangpass.html
static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new MainPass());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerPass);