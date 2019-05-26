//
// Created by suz305 on 2/26/19.
//

#include "MainPass.h"

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

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }

        std::string getName(Value *value){
            if (value->hasName()){
                return value->getName();
            } else{
                auto addr = reinterpret_cast<std::uintptr_t>(value);
                if (this->naming_map.find(addr) == naming_map.end())
                {
                    this->naming_map[addr] = ++naming_index;
                }
                return std::to_string(this->naming_map[addr]);
            }
        }

        void printOperandNames(Instruction &ins)
        {
            for(auto i = ins.op_begin(); i!=ins.op_end();++i )
            {
                outs()<< this->getName(i->get())<<" ";
            }
            outs()<<"\n";
        }

        std::string buildLavaVarName(int64_t bug_no)
        {
            return std::string("_lava_") + std::to_string(bug_no);
        }

        NodePtr handleGEP(GetElementPtrInst &GEPIns)
        {
            //TODO: add SVF support
            outs()<<"in handelGEP:\n";
            this->printOperandNames(GEPIns);
        }

        Instruction* findWpEnd(Function& function) {
            Instruction* ret = nullptr;
            for (auto& bb:function) {
                for (auto ins = bb.rbegin(); ins != bb.rend(); ++ins) {
                    auto opcode = ins->getOpcode();
                    if (opcode == Instruction::Call) {
                        auto *callInst = dyn_cast<CallInst>(&*ins);
                        auto func = callInst->getCalledFunction();
                        auto funcName = func->getName();
                        outs()<<funcName<<"\n";
                        if (funcName == "_wp_end") {
                            ret = &(*ins);
                            break;
                        }
                    }
                }
            }
            return ret;
        }


        void newHandleFunctionCall(Function& function) {

            for (scc_iterator<Function *> BB = scc_begin(&function), BBE = scc_end(&function); BB != BBE; ++BB) {

            }
        }

        std::list<NodePtr> handleFunctionCall(Function& function) {
            for (scc_iterator<Function *> BB = scc_begin(&function), BBE = scc_end(&function); BB != BBE; ++BB) {
                const std::vector<BasicBlock *> &SCCBBs = *BB;
                outs() << SCCBBs.size()<<"\n";
                for (auto BBI = SCCBBs.begin(); BBI != SCCBBs.end(); ++BBI) {
                    outs() << "In BB:" << (*BBI)->getName() << "\n";
                    this->wpPrinter.setBlockName((*BBI)->getName());
                }
            }

            bool isPureFunc = true;
            bool hasCalculatedWP = false;
            if (this->InWP) {
                hasCalculatedWP = true;
            }
            auto funcAddr = reinterpret_cast<uintptr_t >(&function);
            if (this->visitedFunc.find(funcAddr)!=this->visitedFunc.end() && this->visitedFunc[funcAddr]==Pure){
                return this->PureFuncConstaintSet[funcAddr];
            }

            NodePtr expr;
            std::list<NodePtr> constraint_list;
            outs() << "In Function:" << function.getName() << "()\n";
            this->wpPrinter.setFuncName(function.getName());
            if (function.isDeclaration()) {
                return std::list<NodePtr>();
            }

            Instruction *start_ins = nullptr;
            BasicBlock *start_block = nullptr;
            for (auto& bb:function) {
                for (auto ins = bb.rbegin(); ins != bb.rend(); ++ins) {
                    auto opcode = ins->getOpcode();
                    if (opcode == Instruction::Call) {
                        auto *callInst = dyn_cast<CallInst>(&*ins);
                        auto func = callInst->getCalledFunction();
                        auto funcName = func->getName();
                        outs()<<funcName<<"\n";
                        if (funcName == "_wp_end") {
                            start_ins = &(*ins);
                            start_block = &bb;
                            break;
                        }
                    }
                }
            }

            if (!start_ins)
                return constraint_list;

            auto prev_var = start_ins->getPrevNode();
            expr = Node::CreateBinOp(Node::CreateVar(prev_var),
                                     Node::CreateConst(std::string("1234567")),
                                     WpExpr::LT);

            std::queue<std::tuple<Instruction*, std::list<NodePtr> > > workQueue;
            std::list<NodePtr> start_nodelist = {expr};
            workQueue.push(std::make_tuple(start_ins, start_nodelist));
            std::unordered_map<std::uintptr_t, FuncVisitState > visitedBasicBlocks;

            while (!workQueue.empty()) {
                auto headvalue = workQueue.front();
                workQueue.pop();
                auto ins = std::get<0>(headvalue);
                auto li = std::get<1>(headvalue);
                for (auto i = ins;i!= nullptr;i = i->getPrevNode()) {
                    //outs()<<"ins ite: ";
                    i->print(outs());
                    outs()<<"\n";
                    handleInstructions(li, *i, isPureFunc, function.getName());
                    printConstraints(li, *i);
                }
                auto bb = start_ins->getParent();
                for (auto it = pred_begin(bb), et = pred_end(bb); it != et; ++it) {
                    outs() << "pred bbs: " << (*it)->getName()<<"\n";
                }

            }

            EliminateUnsatConstraints(constraint_list);
            if (hasCalculatedWP) {
                if (isPureFunc) {
                    this->visitedFunc[funcAddr] = Pure;
                    this->PureFuncConstaintSet[funcAddr] = constraint_list;
                }
                else
                    this->visitedFunc[funcAddr] = NotPure;
            }
            return constraint_list;
        }

        void printConstraints(const std::list<NodePtr> &constraint_list, const Instruction &instruction) {
            if (!constraint_list.empty()) {
                std::string wpstr_infile = "";
                for (auto x:constraint_list) {
                    auto wpstring = x->ToString();
                    outs() << wpstring << " or \n";
                    wpstr_infile += wpstring + " or ";
                }
                std::string inststr;
                raw_string_ostream rso(inststr);
                instruction.print(rso);
                wpPrinter.emit(inststr, wpstr_infile);
            }
        }

        void EliminateUnsatConstraints(std::list<NodePtr> &constraint_list) const {
            SolverContext sc;
            solver s(sc.Context);
            /*for (auto e:constraint_list) {
                std::cout<<e->ToString()<<"\n";
                s.add(sc.WpExprToZ3Expr(e));
                auto ret = s.check();
                //sc.Reset();
                s.reset();
                std::cout<<(ret==unsat)<<"\n";
            }*/
            constraint_list.remove_if([&sc, &s](NodePtr &e) {
                //std::cout<<e->ToString();
                s.add(sc.WpExprToZ3Expr(e));
                auto ret = s.check();
                s.reset();
                sc.Reset();
                //std::cout<<" "<< (ret == unsat) <<"\n";
                return ret == unsat;
            });
        }

        bool runOnModule(Module &M) override {
            std::cout<<"init\n";
            this->modulename = std::string(M.getName());
            this->wpPrinter.open(this->modulename+std::string("_wp.md"));
            for (Function &F : M) {
                auto ret = this->handleFunctionCall(F);
                if (!ret.empty()){
                    for (auto expr:ret) {
                        outs() << expr->ToString() << "\n";
                    }
                }
            }
            this->wpPrinter.close();
            return false;
        }

        void handleInstructions(std::list<NodePtr> &constraint_list, Instruction &instruction, bool &isPureFunc,
                            std::string FuncName);
    };
}

char MainPass::ID = 0;

void MainPass::handleInstructions(std::list<NodePtr> &constraint_list,
                                  Instruction &instruction, bool &isPureFunc, std::string FuncName) {
    auto opcode = instruction.getOpcode();
    switch (opcode) {
        // Terminator instructions
        case Instruction::Ret: {//TODO: handle same varibale names in different scopes
            auto retins = cast<ReturnInst>(&instruction);
            handleRet(*retins, constraint_list);
            break;
        }

        case Instruction::Br: {
            auto brins = cast<BranchInst>(&instruction);
            outs() << "Number of ops: " << brins->getNumOperands() << "\n";
            outs() << "Number of succs: " << brins->getNumSuccessors() << "\n";
            handleBranch(*brins, constraint_list);
        }
            break;

        case Instruction::Switch: {//O1 won't generate switch
            outs() << "unimplemented instr\n";
            break;
        }

        case Instruction::IndirectBr: {
            outs() << "unimplemented instr\n";
            break;
        }

        case Instruction::Call:  {
            //TODO: handle variable length args and lazy args
            //TODO: handle same variable names in different scopes
            auto callins = cast<CallInst>(&instruction);
            auto lhs = callins->getName();
            outs() << "call site values: ";
            this->printOperandNames(*callins);
            outs() << lhs << "\n";
            auto func = callins->getCalledFunction();
            /*if (func->getName() == "_wp_begin") {
                outs() << "_wp_begin\n";
                this->InWP = false;
                EliminateUnsatConstraints(constraint_list);
                continue;
            }*/
            outs() << "#arg: " << func->arg_size() << "\n";
            outs() << "call site args: ";
            for (Argument &i :func->args()) {
                outs() << i.getName() << " ";
            }
            outs() << "\n";
            if (func->isDeclaration()) {
                break;
            }
            std::list<NodePtr> udexpr = this->handleFunctionCall(*func);
            if (this->visitedFunc[reinterpret_cast<uintptr_t >(func)]==NotPure) {
                isPureFunc = false;
            }
            this->wpPrinter.setFuncName(FuncName);
            if (!udexpr.empty()) {
                auto p = func->arg_begin();
                auto q = callins->op_begin();
                for (; p != func->arg_end() && q != callins->op_end(); p++, q++) {
                    auto arg_val = HandleConstOrVar(*q);
                    outs() << "substitute call sites: " << p->getName() << " "
                           << arg_val->ToString() << "\n";
                    Node::substitute(udexpr, &(*p), arg_val);
                }
                //outs() << "substituted udexpr:" << udexpr->ToString() << "\n";
                Node::fillUndeterminedPredicate(udexpr, constraint_list, &instruction);
                constraint_list = udexpr;
            }
            break;
        }
        case Instruction::Invoke: {
            outs() << "unimplemented instr\n";
            break;
        }

        case Instruction::Resume: {
            outs() << "unimplemented instr\n";
            break;
        }
            // Unary Operations

            // Binary Operations
        case Instruction::Add:
        case Instruction::FAdd:
        case Instruction::Sub:
        case Instruction::FSub:
        case Instruction::Mul:
        case Instruction::FMul:
        case Instruction::UDiv:
        case Instruction::SDiv:
        case Instruction::FDiv:
        case Instruction::URem:
        case Instruction::SRem:
        case Instruction::FRem: {
            auto binins = cast<BinaryOperator>(&instruction);
            handleBinaryOperator(*binins, constraint_list);

            //outs()<<"Op0 Name: "<<instruction.getOperand(0)->getName()<<"\n";
            break;
        }
//todo: bitwise operations
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor: {
            auto bitwiseinst = cast<BinaryOperator>(&instruction);
            auto width = bitwiseinst->getType()->getIntegerBitWidth();
            if (width == 1) {
                handleLogicOp(*bitwiseinst, constraint_list);
            }
            outs() << "bit width: " << std::to_string(width) << "\n";
            break;
        }

//TODO: vectors?

            // Memory
        case Instruction::Alloca: {
            outs() << "unimplemented instr\n";
            break;
        }
        case Instruction::GetElementPtr: {
            auto gepins = cast<GetElementPtrInst>(&instruction);
            handleGetElementPtr(*gepins, constraint_list);
            break;
        }
        case Instruction::Load: {
            isPureFunc = false;
            auto loadins = cast<LoadInst>(&instruction);
            outs() << "In loadIns: \n";
            printOperandNames(instruction);
            handleLoad(*loadins, constraint_list);
            break;
        }

        case Instruction::Store: {
            isPureFunc = false;
            outs() << "unimplemented instr\n";
            break;
        }
        case Instruction::Fence: {
            outs() << "unimplemented instr\n";
            break;
        }

            //conversion
        case Instruction::Trunc:
        case Instruction::FPTrunc:
        case Instruction::FPExt:
        case Instruction::ZExt:
        case Instruction::SExt: {
            //Node::substitute(expr, &instruction,
            //                 Node::CreateVar(instruction.getOperand(0)));
            //TODO: should we add the semantics of floating point numbers into constraints?
            break;
        }

        case Instruction::UIToFP:
        case Instruction::SIToFP:
        case Instruction::FPToUI:
        case Instruction::FPToSI: {
            auto castinst = cast<CastInst>(&instruction);
            handleCast(*castinst, constraint_list);
            //TODO: generate to_real and to_int
            break;
        }

        case Instruction::PtrToInt:
        case Instruction::IntToPtr:
        case Instruction::BitCast:
        case Instruction::AddrSpaceCast: {
            outs() << "unimplemented instr\n";
            break;
        }

            //Other

        case Instruction::Select: {//from WP to (cond and WP[aug1/lhs] or not cond and WP[aug2/lhs])
            auto selectins = cast<SelectInst>(&instruction);
            handleSelect(*selectins, constraint_list);
            break;
        }
            /*case Instruction::And:
            case Instruction::Or:
            case Instruction::Xor:*/
        case Instruction::ICmp: //WP[(aug0 predicate aug1)/lhs]
        {
            auto cmpins = cast<ICmpInst>(&instruction);
            handleICmp(*cmpins, constraint_list);
            break;
        }

        case Instruction::VAArg:
        case Instruction::FCmp: {
            auto fcmpinst = cast<FCmpInst>(&instruction);
            handleFCmp(*fcmpinst, constraint_list);
        }
        case Instruction::LandingPad: {
            outs() << "unimplemented instr\n";
            break;
        }
        case Instruction::PHI: {
            printOperandNames(instruction);
            auto phiins = cast<PHINode>(&instruction);
            handlePHI(*phiins, constraint_list);
        }
            break;
        default:
            outs() << "unknown instr ";
            instruction.print(outs());
            outs() << "\n";
            break;
    }

}
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