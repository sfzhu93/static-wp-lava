//
// Created by suz305 on 2/26/19.
//

#include "MainPass.h"

#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/LegacyPassManager.h"
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

    struct MainPass : public FunctionPass {
        static char ID;
        MainPass() : FunctionPass(ID) {}
        WpExpr::Node::NodePtr WP;
        bool InWP;
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

        void printOprandNames(Instruction &ins)
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
                    outs() << "const or var: 0\n";
                    ret = Node::CreateConst(
                            std::to_string(c->getUniqueInteger().getSExtValue()));//TODO: may change to ZExt
                }
                else
                {
                    outs() << "const or var: 1\n";
                    ret = std::make_shared<Node>();//TODO: not implemented!
                }
            }else
            {
                outs() << "const or var: 2\n";
                ret = Node::CreateVar(value->getName());//TODO: would move fuck up here?
                outs() << "const or var: 2 end\n";
            }
            return ret;
        }

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
                        auto opcode = instruction.getOpcode();
                        auto lhs = instruction.getName();
                        outs()<<lhs<<"\n";
                        if (opcode == Instruction::Call)
                        {
                            outs()<<"In Call Instruction\n";
                            auto *callInst = dyn_cast<CallInst>(&instruction);
                            auto func = callInst->getCalledFunction();
                            auto funcName = func->getName();
                            outs() << funcName << "\n";
                            if (funcName == "_wp_begin") {
                                outs()<<"_wp_begin\n";
                                this->InWP = false;
                                //TODO: solve the WP with z3
                            } else if (funcName == "_wp_end") {
                                this->InWP = true;
                                this->WP = Node::CreateBinOp(Node::CreateVar(std::string(wp_init_var)),
                                                             Node::CreateConst(std::string("1234567")),
                                                             std::string("<"));
                                outs()<<"debug-create-wp\n";
                                //_init_var < magic number

                            } else if (funcName == "lava_get") {
                                auto lhs_name = callInst->getName();
                                auto aug = dyn_cast<Constant>(callInst->getArgOperand(0));
                                auto num = aug->getUniqueInteger().getSExtValue();//TODO: may change to ZExt
                                Node::substitute(this->WP,
                                                 lhs_name,
                                                 Node::CreateVar(buildLavaVarName(num)));
                            } else if (funcName == "lava_set") {
                                auto num = dyn_cast<Constant>(callInst->getArgOperand(0))->getUniqueInteger().getSExtValue();
                                auto aug1 = callInst->getArgOperand(1);
                                auto lava_val_node = this->HandleConstOrVar(aug1);
                                Node::substitute(this->WP, buildLavaVarName(num), lava_val_node);
                            }
                        }else if (this->InWP) {
                            outs()<<"InWP: ";
                            switch (opcode) {
                                case Instruction::Ret:
                                    break;
                                case Instruction::Br:
                                    break;
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
                                case Instruction::Sub:
                                case Instruction::Mul:
                                case Instruction::UDiv:
                                case Instruction::SDiv:
                                case Instruction::URem:
                                case Instruction::SRem:
                                case Instruction::Add: {
                                    outs()<<"debug0\n";
                                    auto binins = cast<BinaryOperator>(&instruction);
                                    outs()<<"debug1\n";
                                    auto aug0 = this->HandleConstOrVar(binins->getOperand(0));
                                    auto aug1 = this->HandleConstOrVar(binins->getOperand(1));
                                    outs()<<"debug2\n";
                                    auto op = std::string(opcode2Name(binins->getOpcode()));
                                    Node::substitute(this->WP, lhs, Node::CreateBinOp(std::move(aug0), std::move(aug1),
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