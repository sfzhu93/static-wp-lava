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

        std::list<NodePtr> handleFunctionCall(Function& function) {
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
            for (scc_iterator<Function *> BB = scc_begin(&function), BBE = scc_end(&function); BB != BBE; ++BB) {
                const std::vector<BasicBlock *> &SCCBBs = *BB;
                for (auto BBI = SCCBBs.begin(); BBI != SCCBBs.end(); ++BBI) {
                    outs() << "In BB:" << (*BBI)->getName() << "\n";
                    this->wpPrinter.setBlockName((*BBI)->getName());
                    for (BasicBlock::reverse_iterator Ins = (*BBI)->rbegin(); Ins != (*BBI)->rend(); ++Ins) {
                        Instruction &instruction = *Ins;
                        instruction.print(outs());
                        outs() << "\n";
                        auto opcode = instruction.getOpcode();
                        auto lhs = instruction.getName();
                        //outs() << lhs << "\n";
                        if (!this->InWP && opcode == Instruction::Call) {
                            outs() << "In Call Instruction\n";
                            auto *callInst = dyn_cast<CallInst>(&instruction);
                            auto func = callInst->getCalledFunction();
                            auto funcName = func->getName();
                            outs() << funcName << "\n";
                            if (funcName == "_wp_end") {
                                this->InWP = true;
                                hasCalculatedWP = true;
                                auto prev_var = instruction.getPrevNode();
                                expr = Node::CreateBinOp(Node::CreateVar(prev_var),
                                                         Node::CreateConst(std::string("1234567")),
                                                         WpExpr::LT);
                                constraint_list.push_front(expr);
                                //_init_var < magic number
                            }
                        } else if (this->InWP) {
                            outs() << "InWP: ";
                            assert(instruction.getOpcode() == opcode);
                            //opcode = instruction.getOpcode();
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
                                case Instruction::Xor:{
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
                                    this->printOperandNames(instruction);
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
                                    Node::substitute(expr, &instruction,
                                                     Node::CreateVar(instruction.getOperand(0)));
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
                                case Instruction::Call: {//TODO: handle variable length args and lazy args
                                    //TODO: handle same variable names in different scopes
                                    auto callins = cast<CallInst>(&instruction);
                                    auto lhs = callins->getName();
                                    outs() << "call site values: ";
                                    this->printOperandNames(*callins);
                                    outs() << lhs << "\n";
                                    auto func = callins->getCalledFunction();
                                    if (func->getName() == "_wp_begin") {
                                        outs() << "_wp_begin\n";
                                        this->InWP = false;
                                        EliminateUnsatConstraints(constraint_list);
                                        /*for (auto e:constraint_list) {
                                            std::cout<<e->ToString()<<"\n";
                                        }*/
                                        //outs() << expr->ToSMTLanguage() << "\n";
                                        /*SolverContext sc;
                                        auto z3expr = sc.WpExprToZ3Expr(expr);
                                        std::cout<<"z3expr:\n";
                                        std::cout<<z3expr<<"\n";
                                        std::cout<<"---------\n";
                                        tactic t1(sc.Context, "ctx-solver-simplify");
                                        t1 = try_for(t1, 100);
                                        params p(sc.Context);
                                        unsigned tmp = 1;
                                        p.set("timeout",tmp);
                                        t1 = with(t1, p);
                                        goal g(sc.Context);
                                        g.add(z3expr);
                                        auto simplifyResult = t1(g);
                                        std::cout<<simplifyResult<<"\n";
                                        std::ofstream fout(this->modulename+".z3result");
                                        fout<<simplifyResult<<"\n";
                                        fout.close();*/
                                        continue;
                                    }
                                    outs() << "#arg: " << func->arg_size() << "\n";
                                    outs() << "call site args: ";
                                    for (Argument &i :func->args()) {
                                        outs() << i.getName() << " ";
                                    }
                                    outs() << "\n";
                                    if (func->isDeclaration()) {
                                        continue;
                                    }
                                    std::list<NodePtr> udexpr = this->handleFunctionCall(*func);
                                    if (this->visitedFunc[reinterpret_cast<uintptr_t >(func)]==NotPure) {
                                        isPureFunc = false;
                                    }
                                    this->wpPrinter.setFuncName(function.getName());
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
                                    this->printOperandNames(instruction);
                                    auto phiins = cast<PHINode>(&instruction);
                                    handlePHI(*phiins, constraint_list);
                                }
                                    break;
                                default:
                                    outs() << "unknown instr\n";
                                    break;
                            }
                            //instruction.dump();
                            if (!constraint_list.empty()) {
                                std::string wpstr_infile = "";
                                for (auto x:constraint_list) {
                                    auto wpstring = x->ToString();
                                    outs() << wpstring << " or \n";
                                    wpstr_infile+=wpstring+" or ";
                                }
                                std::string inststr;
                                llvm::raw_string_ostream rso(inststr);
                                instruction.print(rso);
                                this->wpPrinter.emit(inststr, wpstr_infile);
                            }
                        }
                    }
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