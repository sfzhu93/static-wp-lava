//
// Created by zsf on 19-3-1.
//

#include "InsHandler.h"
#include "Miscs.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"

using namespace llvm;
using namespace WpExpr;

static const char *strAnd = "and";
static const char *strOr = "or";
static const char *strNot = "not";

/**
 *
 * @param value The value should be either a Constant or an Instruction.
 * @return If the value is an Instruction, it returns the left-hand side value
 * (i.e. the name of the Instruction). Otherwise it returns the literal value
 * of the constant.
 */
Node::NodePtr HandleConstOrVar(Value *value) {
  Node::NodePtr ret;
  if (auto c = dyn_cast<Constant>(value)) {
    auto type = c->getType()->getTypeID();
    switch (type) {
    case Type::IntegerTyID:
      ret = CreateConst(std::to_string(
          c->getUniqueInteger().getSExtValue())); // TODO: may change to ZExt
      break;
    case Type::HalfTyID:
    case Type::FloatTyID:
    case Type::DoubleTyID:
    case Type::X86_FP80TyID:
    case Type::FP128TyID:
    case Type::PPC_FP128TyID: {
      auto value = dyn_cast<ConstantFP>(c);

      ret = CreateConst(
          std::to_string(value->getValueAPF().convertToDouble()));
      break;
    }
    default:
      outs() << "unresolved constant type!\n";
    }
  } else // return the variable name
  {
    ret = CreateVar(value); // TODO: check if move works well here
  }
  return ret;
}

/**
 *
 * @param inst The GetElementPtr instruction.
 * @param expr The reference to the smart pointer of expression to be
 * substituted.
 */
void handleGetElementPtr(GetElementPtrInst &inst, Node::NodePtr &expr) {
  auto a = inst.getOperand(0);
  auto nops = inst.getNumOperands();
  for (int i = 0; i < nops; ++i) {
    outs() << "GEPOperand: " << inst.getOperand(i)->getName() << "\n";
  }
  Node::NodePtr value;
  if (isa<GlobalVariable>(a) && a->getName() == "lava_val") {
    value = CreateBinOp(CreateVar(std::string("lava_val")),
                        CreateVar(inst.getOperand(2)->getName().str()),
                        WpExpr::Operator::IND);
  } else {
    value = CreateVar("_");
  }
  Substitute(expr, &inst, value);
}

/**
 *
 * @param inst The Load instruction.
 * @param expr The reference to the smart pointer of expression to be
 * substituted.
 */
void handleLoad(LoadInst &inst, Node::NodePtr &expr) {
  Substitute(expr, &inst, CreateVar(inst.getOperand(0)));

  // TODO: add more notations for the address
}

void handleRet(ReturnInst &inst, Node::NodePtr &expr) {
  if (inst.getReturnValue()) {
    expr = CreateUndeterminedPredicate(inst.getReturnValue());
    /*substitute(expr,
                     std::string("call"),//TODO: replace __ret_val__
                     CreateVar(inst.getReturnValue()->getName()));*/

  } else
    expr = Node::NodePtr();
}

void handleBranch(BranchInst &inst, Node::NodePtr &expr) {
  auto cnt = inst.getNumSuccessors();
  if (cnt > 1) // conditional
  {
    auto cond = inst.getOperand(0);
    outs() << inst.getSuccessor(0)->getName() << "\n";
    outs() << inst.getSuccessor(1)->getName() << "\n";
    Substitute(expr, inst.getSuccessor(0), CreateVar(cond));
    Substitute(expr, inst.getSuccessor(1),
                     CreateUniOp(CreateVar(cond), WpExpr::NOT));
  } else { // unconditional. TODO: replace dest name with basic block's name
  }
}

void handleSelect(SelectInst &inst, Node::NodePtr &expr) {
  auto cond = inst.getOperand(0);
  auto aug1 = HandleConstOrVar(inst.getOperand(1));
  auto aug2 = HandleConstOrVar(inst.getOperand(2));
  auto new_wp_left = std::make_shared<Node>(*expr);
  auto new_wp_right = std::make_shared<Node>(*expr);
  Substitute(new_wp_left, &inst, aug1);  // WP-> WP[aug1/lhs]
  Substitute(new_wp_right, &inst, aug2); // WP -> WP[aug2/lhs]
  new_wp_left = CreateBinOp(CreateVar(cond), std::move(new_wp_left),
                                  WpExpr::AND);
  new_wp_right =
      CreateBinOp(CreateUniOp(CreateVar(cond), WpExpr::NOT),
                        std::move(new_wp_right), WpExpr::AND);
  auto new_wp = CreateBinOp(std::move(new_wp_left),
                                  std::move(new_wp_right), WpExpr::OR);
  expr = std::move(new_wp);
}

void handleBinaryOperator(BinaryOperator &inst, Node::NodePtr &expr) {
  auto aug0 = HandleConstOrVar(inst.getOperand(0));
  auto aug1 = HandleConstOrVar(inst.getOperand(1));
  auto op =
      opcode2WpExprOp(inst.getOpcode()); // std::string((inst.getOpcode()));
  Substitute(expr, &inst,
                   CreateBinOp(std::move(aug0), std::move(aug1), op));
}

void handleICmp(ICmpInst &inst, Node::NodePtr &expr) {
  auto llvmpred = inst.getUnsignedPredicate();
  Node::NodePtr cond;
  if (llvmpred == FCmpInst::FCMP_FALSE) {
    cond = CreateConst("false");
  } else if (llvmpred == FCmpInst::FCMP_TRUE) {
    cond = CreateConst("true");
  } else {
    auto aug0 = HandleConstOrVar(inst.getOperand(0));
    auto aug1 = HandleConstOrVar(inst.getOperand(1));
    auto predicate = predicate2WpExprOp(llvmpred);
    cond = CreateBinOp(std::move(aug0), std::move(aug1), predicate);
  }
  Substitute(expr, &inst, cond);
}

void handlePHI(PHINode &inst, Node::NodePtr &expr) {
  auto lhs = inst.getName();
  auto cnt = inst.getNumIncomingValues();
  if (cnt == 1) {
    outs() << "This is not supposed to happen.\n"; // TODO
  } else {
    // first handling the first two incoming values
    auto cond1 = inst.getIncomingBlock(0);
    auto aug1 = HandleConstOrVar(inst.getOperand(0));
    auto new_wp = std::make_shared<Node>(*expr);
    Node::NodePtr new_wp_right = std::make_shared<Node>();
    Substitute(new_wp, &inst, aug1); // WP-> WP[aug1/lhs]
    new_wp = CreateBinOp(CreateVar(cond1), std::move(new_wp),
                               WpExpr::AND);
    for (unsigned i = 1; i < cnt;
         ++i) { // TODO: design cases to cover different cnt
      auto cond = inst.getIncomingBlock(i);
      auto aug = HandleConstOrVar(inst.getOperand(1));
      new_wp_right = std::make_shared<Node>(*expr);
      Substitute(new_wp_right, &inst, aug);
      new_wp_right = CreateBinOp(CreateVar(cond),
                                       std::move(new_wp_right), WpExpr::AND);
      new_wp = CreateBinOp(std::move(new_wp), std::move(new_wp_right),
                                 WpExpr::OR);
    }
    expr = std::move(new_wp);
    outs() << "end of handlPHI\n";
  }
}

void handleCast(CastInst &inst, Node::NodePtr &expr) {
  auto lhs = inst.getName();
  switch (inst.getOpcode()) {
  case Instruction::FPToUI:
  case Instruction::FPToSI: {
    auto aug = HandleConstOrVar(inst.getOperand(0));
    auto op = std::string("to_int");
    Substitute(expr, &inst,
                     CreateUniOp(std::move(aug), WpExpr::TO_INT));
    break;
  }
  case Instruction::UIToFP:
  case Instruction::SIToFP: {
    auto aug = HandleConstOrVar(inst.getOperand(0));
    auto op = std::string("to_real");
    Substitute(expr, &inst,
                     CreateUniOp(std::move(aug), WpExpr::TO_REAL));
    break;
  }
  }
}

void handleLogicOp(BinaryOperator &inst, Node::NodePtr &expr) {
  auto aug0 = HandleConstOrVar(inst.getOperand(0));
  auto aug1 = HandleConstOrVar(inst.getOperand(1));
  WpExpr::Operator op;
  switch (inst.getOpcode()) {
  case Instruction::And:
    op = WpExpr::AND;
    break;
  case Instruction::Or:
    op = WpExpr::OR;
    break;
  case Instruction::Xor:
    op = WpExpr::XOR;
    break;
  default:
    assert(false);
  }
  auto lhs = inst.getName();
  Substitute(expr, &inst,
                   CreateBinOp(std::move(aug0), std::move(aug1), op));
  // TODO: not implemented
}

void handleFCmp(FCmpInst &inst, Node::NodePtr &expr) {
  auto aug0 = HandleConstOrVar(inst.getOperand(0));
  auto aug1 = HandleConstOrVar(inst.getOperand(1));
  auto predicate = predicate2WpExprOp(inst.getPredicate());
  auto cond = CreateBinOp(std::move(aug0), std::move(aug1), predicate);
  auto lhs = inst.getName();
  Substitute(expr, &inst, cond);
}
