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

using namespace llvm;
using namespace WpExpr;

namespace {

    //ModulePass for interprocedural analysis
    struct MainPass : public ModulePass {
        static char ID;
        MainPass() : ModulePass(ID) {}
        WpExpr::Node::NodePtr WP;
        bool InWP = false;
        std::unordered_map<std::uintptr_t, int> naming_map;
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

        Node::NodePtr handleGEP(GetElementPtrInst &GEPIns)
        {
            //TODO: add SVF support
            outs()<<"in handelGEP:\n";
            this->printOperandNames(GEPIns);

        }

        Node::NodePtr handleFunctionCall(Function& function) {
            Node::NodePtr expr;
            auto nullret = Node::CreateVar(std::string(""));
            outs() << "In Function:" << function.getName() << "()\n";
            this->wpPrinter.setFuncName(function.getName());
            if (function.isDeclaration()) {
                return nullret;
            }
            Node::NodePtr tmp_expr = std::make_shared<Node>();

            for (scc_iterator<Function *> BB = scc_begin(&function), BBE = scc_end(&function); BB != BBE; ++BB) {
                const std::vector<BasicBlock *> &SCCBBs = *BB;
                for (std::vector<BasicBlock *>::const_iterator BBI = SCCBBs.begin(); BBI != SCCBBs.end(); ++BBI) {
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
                                auto prev_var_name = instruction.getPrevNode()->getName();
                                this->WP = Node::CreateBinOp(Node::CreateVar(std::string(prev_var_name)),
                                                             Node::CreateConst(std::string("1234567")),
                                                             std::string("<"));
                                expr = this->WP;
                                //_init_var < magic number

                            }
                        } else if (this->InWP) {
                            outs() << "InWP: ";
                            assert(instruction.getOpcode() == opcode);
                            //opcode = instruction.getOpcode();
                            switch (opcode) {
                                // Terminator instructions
                                case Instruction::Ret: {//TODO: handle same varibale names in different scopes
                                    tmp_expr = Node::CreateVar("_ret_");
                                    auto retins = cast<ReturnInst>(&instruction);
                                    handleRet(*retins, expr);
                                    break;
                                }

                                case Instruction::Br: {
                                    auto brins = cast<BranchInst>(&instruction);
                                    outs() << "Number of ops: " << brins->getNumOperands() << "\n";
                                    outs() << "Number of succs: " << brins->getNumSuccessors() << "\n";
                                    handleBranch(*brins, expr);
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
                                    handleBinaryOperator(*binins, expr);

                                    //outs()<<"Op0 Name: "<<instruction.getOperand(0)->getName()<<"\n";
                                    break;
                                }
//todo: bitwise operations
                                case Instruction::And:
                                case Instruction::Or: {
                                    auto bitwiseinst = cast<BinaryOperator>(&instruction);
                                    auto width = bitwiseinst->getType()->getIntegerBitWidth();
                                    if (width == 1) {
                                        handleLogicOp(*bitwiseinst, expr);
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
                                    handleGetElementPtr(*gepins, expr);
                                    break;
                                }
                                case Instruction::Load: {
                                    auto loadins = cast<LoadInst>(&instruction);
                                    outs() << "In loadIns: \n";
                                    this->printOperandNames(instruction);
                                    handleLoad(*loadins, expr);
                                    break;
                                }

                                case Instruction::Store: {
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
                                    Node::substitute(expr, instruction.getName(),
                                                     Node::CreateVar(instruction.getOperand(0)->getName()));
                                    //TODO: should we add the semantics of floating point numbers into constraints?
                                    break;
                                }

                                case Instruction::UIToFP:
                                case Instruction::SIToFP:
                                case Instruction::FPToUI:
                                case Instruction::FPToSI: {
                                    auto castinst = cast<CastInst>(&instruction);
                                    handleCast(*castinst, expr);
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
                                        outs() << this->WP->ToSMTLanguage() << "\n";
                                        continue;
                                        //TODO: solve the WP with z3
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
                                    Node::NodePtr udexpr = this->handleFunctionCall(*func);
                                    if (udexpr) {
                                        auto p = func->arg_begin();
                                        auto q = callins->op_begin();
                                        for (; p != func->arg_end() && q != callins->op_end(); p++, q++) {
                                            auto arg_val = HandleConstOrVar(*q);
                                            outs() << "substitute call sites: " << p->getName() << " "
                                                   << arg_val->ToString() << "\n";
                                            Node::substitute(udexpr, p->getName(), arg_val);
                                        }
                                        outs() << "substituted udexpr:" << udexpr->ToString() << "\n";
                                        Node::fillUndeterminedPredicate(udexpr, expr, lhs);
                                        expr = udexpr;
                                    }
                                    //TODO: define a var called __ret_val__ for substitution
                                    break;
                                }
                                case Instruction::Select: {//from WP to (cond and WP[aug1/lhs] or not cond and WP[aug2/lhs])
                                    auto selectins = cast<SelectInst>(&instruction);
                                    handleSelect(*selectins, expr);
                                    break;
                                }
                                    /*case Instruction::And:
                                    case Instruction::Or:
                                    case Instruction::Xor:*/
                                case Instruction::ICmp: //WP[(aug0 predicate aug1)/lhs]
                                {
                                    auto cmpins = cast<ICmpInst>(&instruction);
                                    handleICmp(*cmpins, expr);
                                    break;
                                }

                                case Instruction::VAArg:
                                case Instruction::FCmp: {
                                    auto fcmpinst = cast<FCmpInst>(&instruction);
                                    handleFCmp(*fcmpinst, expr);
                                }
                                case Instruction::LandingPad: {
                                    outs() << "unimplemented instr\n";
                                    break;
                                }


                                case Instruction::PHI: {
                                    this->printOperandNames(instruction);
                                    auto phiins = cast<PHINode>(&instruction);
                                    handlePHI(*phiins, expr);
                                }
                                    break;
                                default:
                                    outs() << "unknown instr\n";
                                    break;
                            }
                            //instruction.dump();
                            auto wpstring = expr->ToString();
                            outs() << "WP: " << wpstring << "\n";
                            std::string inststr;
                            llvm::raw_string_ostream rso(inststr);
                            instruction.print(rso);
                            this->wpPrinter.emit(inststr, wpstring);
                        }
                    }
                }
            }
            return expr;
        }

        bool runOnModule(Module &M) override {
            std::cout<<"init\n";
            this->wpPrinter.open(std::string(M.getName())+std::string("_wp.md"));
            for (Function &F : M) {
                this->handleFunctionCall(F);
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