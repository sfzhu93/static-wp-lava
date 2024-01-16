//
// Created by suz305 on 2/26/19.
//

#include "MainPass.h"

#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"

#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "InsHandler.h"
#include "Miscs.h"
#include "WpExpr.h"
#include "WpPrinter.h"
#include <iostream>
// #include <z3++.h>
#include "MyZ3Helper.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <unordered_map>

using namespace llvm;
using namespace WpExpr;

namespace {

// ModulePass for interprocedural analysis

std::string MainPass::getName(Value *value) {
  if (value->hasName()) {
    return value->getName().str();
  } else {
    auto addr = reinterpret_cast<std::uintptr_t>(value);
    if (this->naming_map.find(addr) == naming_map.end()) {
      this->naming_map[addr] = ++naming_index;
    }
    return std::to_string(this->naming_map[addr]);
  }
}

void MainPass::printOperandNames(Instruction &ins) {
  for (auto i = ins.op_begin(); i != ins.op_end(); ++i) {
    outs() << this->getName(i->get()) << " ";
  }
  outs() << "\n";
}

std::string MainPass::buildLavaVarName(int64_t bug_no) {
  return std::string("_lava_") + std::to_string(bug_no);
}

Node::NodePtr MainPass::handleGEP(GetElementPtrInst &GEPIns) {
  // TODO: add SVF support
  outs() << "in handelGEP:\n";
  this->printOperandNames(GEPIns);
}

Node::NodePtr MainPass::handleFunctionCall(Function &function) {
  Node::NodePtr expr;
  outs() << "In Function:" << function.getName() << "()\n";
  this->wpPrinter.setFuncName(function.getName().str());
  if (function.isDeclaration()) {
    return expr;
  }
  Node::NodePtr tmp_expr = std::make_shared<Node>();

  for (scc_iterator<Function *> BB = scc_begin(&function),
                                BBE = scc_end(&function);
       BB != BBE; ++BB) {
    const std::vector<BasicBlock *> &SCCBBs = *BB;
    for (auto BBI = SCCBBs.begin(); BBI != SCCBBs.end(); ++BBI) {
      outs() << "In BB:" << (*BBI)->getName() << "\n";
      this->wpPrinter.setBlockName((*BBI)->getName().str());
      for (BasicBlock::reverse_iterator Ins = (*BBI)->rbegin();
           Ins != (*BBI)->rend(); ++Ins) {
        Instruction &instruction = *Ins;
        instruction.print(outs());
        outs() << "\n";
        auto opcode = instruction.getOpcode();
        auto lhs = instruction.getName();
        // outs() << lhs << "\n";
        if (!this->InWP && opcode == Instruction::Call) {
          outs() << "In Call Instruction\n";
          auto *callInst = dyn_cast<CallInst>(&instruction);
          auto func = callInst->getCalledFunction();
          auto funcName = func->getName();
          outs() << funcName << "\n";
          if (funcName == "_wp_end") {
            this->InWP = true;
            auto prev_var = instruction.getPrevNode();
            expr = CreateBinOp(CreateVar(prev_var),
                               CreateConst(std::string("1234567")), WpExpr::LT);
            //_init_var < magic number
          }
        } else if (this->InWP) {
          outs() << "InWP: ";
          assert(instruction.getOpcode() == opcode);
          // opcode = instruction.getOpcode();
          switch (opcode) {
          // Terminator instructions
          case Instruction::Ret: { // TODO: handle same varibale names in
            // different scopes
            tmp_expr = CreateVar(&instruction);
            auto retins = cast<ReturnInst>(&instruction);
            handleRet(*retins, expr);
            break;
          }

          case Instruction::Br: {
            auto brins = cast<BranchInst>(&instruction);
            outs() << "Number of ops: " << brins->getNumOperands() << "\n";
            outs() << "Number of succs: " << brins->getNumSuccessors() << "\n";
            handleBranch(*brins, expr);
          } break;

          case Instruction::Switch: { // O1 won't generate switch
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

            // outs()<<"Op0 Name: "<<instruction.getOperand(0)->getName()<<"\n";
            break;
          }
            // todo: bitwise operations
          case Instruction::And:
          case Instruction::Or:
          case Instruction::Xor: {
            auto bitwiseinst = cast<BinaryOperator>(&instruction);
            auto width = bitwiseinst->getType()->getIntegerBitWidth();
            if (width == 1) {
              handleLogicOp(*bitwiseinst, expr);
            }
            outs() << "bit width: " << std::to_string(width) << "\n";
            break;
          }

            // TODO: vectors?

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

            // conversion
          case Instruction::Trunc:
          case Instruction::FPTrunc:
          case Instruction::FPExt:
          case Instruction::ZExt:
          case Instruction::SExt: {
            Substitute(expr, &instruction,
                       CreateVar(instruction.getOperand(0)));
            // TODO: should we add the semantics of floating point numbers into
            // constraints?
            break;
          }

          case Instruction::UIToFP:
          case Instruction::SIToFP:
          case Instruction::FPToUI:
          case Instruction::FPToSI: {
            auto castinst = cast<CastInst>(&instruction);
            handleCast(*castinst, expr);
            // TODO: generate to_real and to_int
            break;
          }

          case Instruction::PtrToInt:
          case Instruction::IntToPtr:
          case Instruction::BitCast:
          case Instruction::AddrSpaceCast: {
            outs() << "unimplemented instr\n";
            break;
          }

            // Other
          case Instruction::Call: { // TODO: handle variable length args and
            // lazy args
            // TODO: handle same variable names in different scopes
            auto callins = cast<CallInst>(&instruction);
            auto lhs = callins->getName();
            outs() << "call site values: ";
            this->printOperandNames(*callins);
            outs() << lhs << "\n";
            auto func = callins->getCalledFunction();
            if (func->getName() == "_wp_begin") {
              outs() << "_wp_begin\n";
              this->InWP = false;
              outs() << expr->ToSMTLanguage() << "\n";
              SolverContext sc;
              auto z3expr = sc.WpExprToZ3Expr(expr);
              std::cout << "z3expr:\n";
              std::cout << z3expr << "\n";
              std::cout << "---------\n";
              tactic t1(sc.Context, "ctx-solver-simplify");
              // t1 = try_for(t1, 100);
              /*params p(sc.Context);
              unsigned tmp = 1;
              p.set("timeout",tmp);
              t1 = with(t1, p);*/
              goal g(sc.Context);
              g.add(z3expr);
              try {
                std::cout << "Simplify result:\n";
                auto simplifyResult = t1.apply(g);
                std::cout << simplifyResult[0].as_expr() << "\n";
                std::ofstream fout(this->modulename + ".z3result");
                fout << simplifyResult << "\n";
                fout.close();
                params p(sc.Context);
                p.set("timeout", 5000u); // Timeout in milliseconds
                z3::solver s(sc.Context);
                s.add(simplifyResult[0].as_expr());
                std::cout << "Check result:\n";
                switch (s.check()) {
                case z3::sat: {
                  std::cout << "Satisfiable. \n";
                } break;
                case z3::unsat:
                  std::cout << "Unsatisfiable" << std::endl;
                  break;
                case z3::unknown:
                  std::cout << "Unknown" << std::endl;
                  break;
                }
              } catch (const z3::exception &ex) {
                std::cerr << "Z3 Exception: " << ex << std::endl;
              }
              continue;
            }
            outs() << "#arg: " << func->arg_size() << "\n";
            outs() << "call site args: ";
            for (Argument &i : func->args()) {
              outs() << i.getName() << " ";
            }
            outs() << "\n";
            if (func->isDeclaration()) {
              continue;
            }
            Node::NodePtr udexpr = this->handleFunctionCall(*func);
            this->wpPrinter.setFuncName(function.getName().str());
            if (udexpr) {
              auto p = func->arg_begin();
              auto q = callins->op_begin();
              for (; p != func->arg_end() && q != callins->op_end(); p++, q++) {
                auto arg_val = HandleConstOrVar(*q);
                outs() << "substitute call sites: " << p->getName() << " "
                       << arg_val->ToString() << "\n";
                Substitute(udexpr, p, arg_val);
              }
              outs() << "substituted udexpr:" << udexpr->ToString() << "\n";
              Node::fillUndeterminedPredicate(udexpr, expr, &instruction);
              expr = udexpr;
            }
            break;
          }
          case Instruction::Select: { // from WP to (cond and WP[aug1/lhs] or
            // not cond and WP[aug2/lhs])
            auto selectins = cast<SelectInst>(&instruction);
            handleSelect(*selectins, expr);
            break;
          }
            /*case Instruction::And:
            case Instruction::Or:
            case Instruction::Xor:*/
          case Instruction::ICmp: // WP[(aug0 predicate aug1)/lhs]
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
          } break;
          default:
            outs() << "unknown instr\n";
            break;
          }
          // instruction.dump();
          if (expr) {
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
  }
  return expr;
}

llvm::PreservedAnalyses MainPass::run(llvm::Module &M,
                                      llvm::ModuleAnalysisManager &AM) {
  std::cout << "init\n";
  this->modulename = std::string(M.getName());
  this->wpPrinter.open(this->modulename + std::string("_wp.md"));
  for (Function &F : M) {
    auto ret = this->handleFunctionCall(F);
    if (ret) {
      outs() << ret->ToString() << "\n";
    }
  }
  this->wpPrinter.close();
  return llvm::PreservedAnalyses::all();
}

} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  // std::cout << "llvmGetPassPluginInfo hook\n";
  return {LLVM_PLUGIN_API_VERSION, "MainPass", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::ModulePassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  // std::cout << "in llvmGetPassPluginInfo callback, current
                  // pass name: " << Name.str() << "\n";
                  if (Name == "wpgen") {
                    FPM.addPass(MainPass());
                    return true;
                  }
                  return false;
                });
          }};
}