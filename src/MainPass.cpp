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
//#include <z3++.h>
#include <unordered_map>

using namespace llvm;
using namespace WpExpr;

namespace {
    //copied from higher version llvm
    StringRef getPredicateName(CmpInst::Predicate Pred) {
        switch (Pred) {
            default:                   return "unknown";
            case FCmpInst::FCMP_FALSE: return "false";
            case FCmpInst::FCMP_OEQ:   return "oeq";
            case FCmpInst::FCMP_OGT:   return "ogt";
            case FCmpInst::FCMP_OGE:   return "oge";
            case FCmpInst::FCMP_OLT:   return "olt";
            case FCmpInst::FCMP_OLE:   return "ole";
            case FCmpInst::FCMP_ONE:   return "one";
            case FCmpInst::FCMP_ORD:   return "ord";
            case FCmpInst::FCMP_UNO:   return "uno";
            case FCmpInst::FCMP_UEQ:   return "ueq";
            case FCmpInst::FCMP_UGT:   return "ugt";
            case FCmpInst::FCMP_UGE:   return "uge";
            case FCmpInst::FCMP_ULT:   return "ult";
            case FCmpInst::FCMP_ULE:   return "ule";
            case FCmpInst::FCMP_UNE:   return "une";
            case FCmpInst::FCMP_TRUE:  return "true";
            case ICmpInst::ICMP_EQ:    return "=";
            case ICmpInst::ICMP_NE:    return "ne";
            case ICmpInst::ICMP_SGT:   return "sgt";
            case ICmpInst::ICMP_SGE:   return "sge";
            case ICmpInst::ICMP_SLT:   return "slt";
            case ICmpInst::ICMP_SLE:   return "sle";
            case ICmpInst::ICMP_UGT:   return "ugt";
            case ICmpInst::ICMP_UGE:   return "uge";
            case ICmpInst::ICMP_ULT:   return "ult";
            case ICmpInst::ICMP_ULE:   return "ule";
        }
    }

    const char * opcode2Name(unsigned opcode){
        switch (opcode)
        {
            case Instruction::Sub:
                return "-";
            case Instruction::Mul:
                return "*";
            case Instruction::UDiv:
                return "/";
            case Instruction::SDiv:
                return "/";
            case Instruction::URem:
                return "mod";
            case Instruction::SRem:
                return "mod";
            case Instruction::Add:
                return "+";
            default:
                return Instruction::getOpcodeName(opcode);
        }
    }

    //std::unordered_map<CmpInst::Predicate, const char *> predicate2name;
    /* =
            {
                    {ICmpInst::ICMP_EQ,"ICMP_EQ"},
                    {ICmpInst::ICMP_NE,"ICMP_NE"},
                    {ICmpInst::ICMP_UGT,"ICMP_UGT"},
                    {ICmpInst::ICMP_UGE,"ICMP_UGE"},
                    {ICmpInst::ICMP_ULT,"ICMP_ULT"},
                    {ICmpInst::ICMP_ULE,"ICMP_ULE"},
                    {ICmpInst::ICMP_SGT,"ICMP_SGT"},
                    {ICmpInst::ICMP_SGE,"ICMP_SGE"},
                    {ICmpInst::ICMP_SLT,"ICMP_SLT"},
                    {ICmpInst::ICMP_SLE,"ICMP_SLE"}
            };*/
    /*static const std::unordered_map<int, const char *> predicate2name =
            {
                    {1,"ICMP_EQ"},
                    {2,"ICMP_NE"},
                    {3,"ICMP_UGT"},
                    {4,"ICMP_UGE"},
                    {5,"ICMP_ULT"},
                    {6,"ICMP_ULE"},
                    {7,"ICMP_SGT"},
                    {8,"ICMP_SGE"},
                    {9,"ICMP_SLT"},
                    {0,"ICMP_SLE"}
            };*/

    //ModulePass for interprocedural analysis
    struct MainPass : public ModulePass {
        static char ID;
        MainPass() : ModulePass(ID) {}
        WpExpr::Node::NodePtr WP;
        bool InWP = false;
        std::unordered_map<std::uintptr_t, int> naming_map;

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


        Node::NodePtr HandleConstOrVar(Value *value)
        {
            Node::NodePtr ret;
            if(auto c = dyn_cast<Constant>(value))
            {
                if (c->getType()->isIntegerTy()) {
                    ret = Node::CreateConst(
                            std::to_string(c->getUniqueInteger().getSExtValue()));//TODO: may change to ZExt
                }
                else
                {
                    ret = std::make_shared<Node>();//TODO: not implemented!
                }
            }else
            {
                ret = Node::CreateVar(value->getName());//TODO: would move fuck up here?
            }
            return ret;
        }

        Node::NodePtr handleGEP(GetElementPtrInst &GEPIns)
        {
            //TODO: add SVF support
            outs()<<"in handelGEP:\n";
            this->printOperandNames(GEPIns);
            auto a = GEPIns.getOperand(0);
            if (isa<GlobalVariable>(a) && a->getName()=="lava_val") {
                return Node::CreateBinOp(
                        Node::CreateVar(std::string("lava_val")),
                        Node::CreateVar(GEPIns.getOperand(2)->getName()),
                        "[]"
                        );
            }
            /*for (auto i = GEPIns.op_begin();i!=GEPIns.op_end();++i)
            {
                if (auto )
            }*/
            return Node::CreateVar("_");
        }

        Node::NodePtr handleFunctionCall(Function& function){
            auto nullret = Node::CreateVar(std::string(""));
            outs() << "In Function:" << function.getName() << "()\n";
            if(function.isDeclaration())
            {
                return nullret;
            }
            Node::NodePtr tmp_expr = std::make_shared<Node>();

            for (scc_iterator<Function *> BB = scc_begin(&function), BBE = scc_end(&function); BB != BBE; ++BB) {
                const std::vector<BasicBlock *> &SCCBBs = *BB;
                for (std::vector<BasicBlock *>::const_iterator BBI = SCCBBs.begin(); BBI != SCCBBs.end(); ++BBI) {
                    outs() << "In BB:" << (*BBI)->getName() << "\n";
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
                            if (funcName == "_wp_begin") {
                                outs() << "_wp_begin\n";
                                this->InWP = false;
                                //TODO: solve the WP with z3
                            } else if (funcName == "_wp_end") {
                                this->InWP = true;
                                this->WP = Node::CreateBinOp(Node::CreateVar(std::string(wp_init_var)),
                                                             Node::CreateConst(std::string("1234567")),
                                                             std::string("<"));
                                //_init_var < magic number

                            } /*else if (funcName == "lava_get") {
                                    auto lhs_name = callInst->getName();
                                    auto aug = dyn_cast<Constant>(callInst->getArgOperand(0));
                                    auto num = aug->getUniqueInteger().getSExtValue();//TODO: may change to ZExt
                                    Node::substitute(this->WP,
                                                     lhs_name,
                                                     Node::CreateVar(buildLavaVarName(num)));
                                } else if (funcName == "lava_set") {
                                    auto num = dyn_cast<Constant>(
                                            callInst->getArgOperand(0))->getUniqueInteger().getSExtValue();
                                    auto aug1 = callInst->getArgOperand(1);
                                    auto lava_val_node = this->HandleConstOrVar(aug1);
                                    Node::substitute(this->WP, buildLavaVarName(num), lava_val_node);
                                }*/
                        }else if (this->InWP) {
                            outs() << "InWP: ";
                            switch (opcode) {
                                case Instruction::GetElementPtr: {
                                    auto gepins = cast<GetElementPtrInst>(&instruction);
                                    auto gep_expr = this->handleGEP(*gepins);
                                    Node::substitute(this->WP, gepins->getName(), gep_expr);
                                    break;
                                }
                                case Instruction::Load: {
                                    auto loadins = cast<LoadInst>(&instruction);
                                    outs()<<"In loadIns: \n";
                                    this->printOperandNames(instruction);
                                    Node::substitute(this->WP, loadins->getName(), Node::CreateVar(loadins->getOperand(0)->getName()));
                                    //TODO: add more notations for the address
                                    break;
                                }
                                case Instruction::Ret: {
                                    tmp_expr = Node::CreateVar("_ret_");
                                    auto retins = cast<ReturnInst>(&instruction);
                                    if (retins->getReturnValue())
                                    {
                                        Node::substitute(this->WP,
                                                std::string("call"),//TODO: replace __ret_val__
                                                Node::CreateVar(retins->getReturnValue()->getName()));

                                    }
                                    break;
                                }
                                case Instruction::Br:
                                    break;
                                case Instruction::Call: {//TODO: handle variable length args and lazy args
                                    auto callins = cast<CallInst>(&instruction);
                                    auto lhs = callins->getName();
                                    outs()<<"call site values: ";
                                    this->printOperandNames(*callins);
                                    outs()<<lhs<<"\n";
                                    auto func = callins->getCalledFunction();
                                    outs()<<"#arg: "<<func->arg_size()<<"\n";
                                    outs()<<"call site args: ";
                                    for (Argument &i :func->args())
                                    {
                                        outs() << i.getName()<<" ";
                                    }
                                    outs()<<"\n";
                                    auto subsitute = this->handleFunctionCall(*func);
                                    auto p = func->arg_begin();
                                    auto q = callins->op_begin();
                                    for (;p!=func->arg_end() && q!= callins->op_end(); p++, q++)
                                    {
                                        auto expr = this->HandleConstOrVar(*q);
                                        outs() <<"substitute call sites: " << p->getName()<< " " << expr->ToString()<<"\n";
                                        Node::substitute(this->WP, p->getName(), expr);
                                    }
                                    //TODO: define a var called __ret_val__ for substitution
                                    break;
                                }
                                case Instruction::Select: {//from WP to (cond and WP[aug1/lhs] or not cond and WP[aug2/lhs])
                                    auto selectins = cast<SelectInst>(&instruction);
                                    auto cond = selectins->getOperand(0)->getName();
                                    auto aug1 = this->HandleConstOrVar(selectins->getOperand(1));
                                    auto aug2 = this->HandleConstOrVar(selectins->getOperand(2));

                                    auto new_wp_left = std::make_shared<Node>(*this->WP);
                                    auto new_wp_right = std::make_shared<Node>(*this->WP);
                                    Node::substitute(new_wp_left, lhs, aug1);// WP-> WP[aug1/lhs]
                                    Node::substitute(new_wp_right, lhs, aug2);//WP -> WP[aug2/lhs]
                                    new_wp_left = Node::CreateBinOp(Node::CreateVar(cond),
                                                                    std::move(new_wp_left),
                                                                    std::string("AND"));
                                    new_wp_right = Node::CreateBinOp(Node::CreateUniOp(
                                            Node::CreateVar(cond),
                                            std::string("NOT")),
                                                                     std::move(new_wp_right),
                                                                     std::string("AND"));
                                    auto new_wp = Node::CreateBinOp(std::move(new_wp_left),
                                                                    std::move(new_wp_right),
                                                                    std::string("OR"));
                                    this->WP = std::move(new_wp);
                                    break;
                                }
                                case Instruction::ZExt:
                                case Instruction::SExt: {
                                    Node::substitute(this->WP, instruction.getName(),
                                            Node::CreateVar(instruction.getOperand(0)->getName()));
                                    break;
                                }
                                case Instruction::Sub:
                                case Instruction::Mul:
                                case Instruction::UDiv:
                                case Instruction::SDiv:
                                case Instruction::URem:
                                case Instruction::SRem:
                                case Instruction::Add: {
                                    auto binins = cast<BinaryOperator>(&instruction);
                                    auto aug0 = this->HandleConstOrVar(binins->getOperand(0));
                                    auto aug1 = this->HandleConstOrVar(binins->getOperand(1));
                                    auto op = std::string(opcode2Name(binins->getOpcode()));
                                    Node::substitute(this->WP, lhs,
                                                     Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                                                       std::move(op)));
                                    //outs()<<"Op0 Name: "<<instruction.getOperand(0)->getName()<<"\n";
                                    break;
                                }
                                    /*case Instruction::And:
                                    case Instruction::Or:
                                    case Instruction::Xor:*/
                                case Instruction::ICmp: //WP[(aug0 predicate aug1)/lhs]
                                {
                                    auto cmpins = cast<ICmpInst>(&instruction);
                                    auto aug0 = HandleConstOrVar(cmpins->getOperand(0));
                                    auto aug1 = HandleConstOrVar(cmpins->getOperand(1));
                                    auto predicate = getPredicateName(cmpins->getUnsignedPredicate());
                                    auto cond = Node::CreateBinOp(std::move(aug0), std::move(aug1),
                                                                  std::string(predicate));
                                    Node::substitute(this->WP, lhs, cond);
                                    break;
                                }

                                case Instruction::PHI:
                                    break;
                                default:
                                    break;
                            }
                            //instruction.dump();
                            outs() << "WP: " << this->WP->ToString() << "\n";
                        }

                    }
                }
            }

        }

        bool runOnModule(Module &M) override {
            for (Function &F : M) {
                this->handleFunctionCall(F);

            }
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