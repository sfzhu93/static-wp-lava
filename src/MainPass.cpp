//
// Created by zsf on 19-6-7.
//

#include "MainPass.h"


char MainPass::ID = 0;

static RegisterPass<MainPass> X("wpgen","Weakest Precondition Analysis");


// http://adriansampson.net/blog/clangpass.html
static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new MainPass());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerPass);

void MainPass::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
}

std::string MainPass::getName(Value *value) {
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

void MainPass::printOperandNames(Instruction &ins) {
    for(auto i = ins.op_begin(); i!=ins.op_end();++i )
    {
        outs()<< this->getName(i->get())<<" ";
    }
    outs()<<"\n";
}

std::string MainPass::buildLavaVarName(int64_t bug_no) {
    return std::string("_lava_") + std::to_string(bug_no);
}

NodePtr MainPass::handleGEP(GetElementPtrInst &GEPIns) {
    //TODO: add SVF support
    outs()<<"in handelGEP:\n";
    this->printOperandNames(GEPIns);
}

Instruction *MainPass::findWpEnd(Function &function) {
    Instruction* ret = nullptr;
    for (auto& bb:function) {
        for (auto ins = bb.rbegin(); ins != bb.rend(); ++ins) {
            auto opcode = ins->getOpcode();
            if (opcode == Instruction::Call) {
                auto *callInst = dyn_cast<CallInst>(&*ins);
                auto func = callInst->getCalledFunction();
                auto funcName = func->getName();
                if (funcName == "_wp_end") {
                    ret = &(*ins);
                    break;
                }
            }
        }
    }
    return ret;
}

NodePtr MainPass::setWp(Instruction *ins) {
    auto prev_var = ins->getPrevNode();
    return Node::CreateBinOp(Node::CreateVar(prev_var),
                             Node::CreateConst(std::string("1234567")),
                             WpExpr::LT);
}

void MainPass::copyToConstraintList(const std::list<NodePtr> &a, std::list<NodePtr> &target) {
    for (NodePtr x:a)
        target.push_back(std::make_shared<Node>(*x));
}

std::list<NodePtr> MainPass::newHandleFunctionCall(Function &function, bool started) {
    if (function.getName()=="f") {
        std::cout<<"in func f\n";
    }
    std::unordered_map<BasicBlock*, std::list<NodePtr> > bb2constraintlist;
    bool isPureFunc = false;
    //started = false;
    Instruction *inst_start;
    if (!started) {
        inst_start = findWpEnd(function);
        if (!inst_start)
            return std::list<NodePtr>();
        outs()<<"found _wp_end in function "<<function.getName()<<"\n";
        auto wp_start = setWp(inst_start);
        auto bb_start = inst_start->getParent();
        std::list<NodePtr> start_cons_list;
        start_cons_list.push_back(wp_start);
        bb2constraintlist[bb_start] = start_cons_list;
    } else {
        inst_start = &function.back().back();
    }
    BasicBlock *last = nullptr;
    for (scc_iterator<Function *> sccIterator = scc_begin(&function), sccEnd = scc_end(&function); sccIterator != sccEnd; ++sccIterator) {
        //(auto bbIterator = sccIterator->rbegin();bbIterator!=sccIterator->rend();++bbIterator) {
        for (auto bbIterator = sccIterator->begin();bbIterator!=sccIterator->end();bbIterator++){
            auto bb = *bbIterator;
            std::list<NodePtr> constraint_set = bb2constraintlist[bb];
            outs()<<"In BB: " << bb->getName() <<"\n";
            for (auto succ_bb_it = succ_begin(bb), end = succ_end(bb);succ_bb_it!=end;succ_bb_it++) {
                BasicBlock *bbptr = *succ_bb_it;
                outs() << "pred bb:" << bbptr->getName() << "\n";
                //copyToConstraintList(bb2constraintlist[bbptr], constraint_set);
            }
            for (auto insIterator = bb->rbegin();insIterator!=bb->rend();++insIterator) {
                auto &ins = *insIterator;
                if (started) {
                    if (&ins == &bb->front()) {
                        if (ins.getOpcode() == Instruction::PHI) {
                            auto inst = cast<PHINode>(&ins);
                            auto cnt = inst->getNumIncomingValues();
                            if (cnt == 1) {
                                outs() << "This is not supposed to happen.\n";//TODO
                            } else {
                                for (auto i = 0;i<cnt;++i) {
                                    auto cond = inst->getIncomingBlock(i);
                                    std::list<NodePtr> &tmp_list = bb2constraintlist[cond];
                                    auto aug = HandleConstOrVar(inst->getOperand(i));
                                    for (auto expr:constraint_set) {
                                        auto new_wp = std::make_shared<Node>(*expr);
                                        Node::substitute(new_wp, inst, aug);
                                        new_wp = Node::CreateBinOp(Node::CreateVar(cond),
                                                                   std::move(new_wp),
                                                                   WpExpr::AND);
                                        tmp_list.push_back(new_wp);
                                    }
                                }
                                outs() <<"end of handling PHI\n";
                            }
                        } else {
                            outs()<<"at the first ins: ";
                            ins.print(outs());
                            outs()<<"\n";
                            auto inst = &ins;
                            handleInstructions(constraint_set, ins, isPureFunc, function.getName());
                            if (bb != &function.front()) {
                                auto prevblock = bb->getSinglePredecessor();//TODO: may change to unique pred
                                std::list<NodePtr> &tmp_list = bb2constraintlist[prevblock];
                                for (auto expr:constraint_set) {
                                    auto new_wp = std::make_shared<Node>(*expr);
                                    std::cout<<"push_back "<<new_wp->ToString()<<" to "<<prevblock->getName().str() <<"\n";
                                    tmp_list.push_back(new_wp);
                                }
                            }
                        }
                    }else {
                        handleInstructions(constraint_set, ins, isPureFunc, function.getName());
                    }
                    printConstraints(constraint_set, ins);
                }
                if (&ins == inst_start) {
                    started = true;
                }
            }
            //EliminateUnsatConstraints(constraint_set);
            printConstraints(constraint_set, bb->front());
            bb2constraintlist[bb] = constraint_set;
            //outs()<<"bb name: "<<bb->getName()<<"\n";
            //printConstraints(constraint_set, bb->front());
            last = bb;
        }
    }
    return bb2constraintlist[last];
}

void MainPass::printConstraints(const std::list<NodePtr> &constraint_list, const Instruction &instruction) {
    std::string inststr;
    raw_string_ostream rso(inststr);
    instruction.print(rso);
    outs() << inststr << "\n";
    if (!constraint_list.empty()) {
        std::string wpstr_infile = "";
        for (auto x:constraint_list) {
            auto wpstring = x->ToString();
            outs() << wpstring << " or \n";
            wpstr_infile += wpstring + " or ";
        }


        wpPrinter.emit(inststr, wpstr_infile);
    }
}

void MainPass::EliminateUnsatConstraints(std::list<NodePtr> &constraint_list) const {
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

bool MainPass::runOnModule(Module &M) {
    this->modulename = std::string(M.getName());
    this->wpPrinter.open(this->modulename+std::string("_wp.md"));
    /*for (Function &F : M) {
        auto ret = this->handleFunctionCall(F);
        if (!ret.empty()){
            for (auto expr:ret) {
                outs() << expr->ToString() << "\n";
            }
        }
    }*/
    for (auto &F : M) {
        newHandleFunctionCall(F, false);
    }
    this->wpPrinter.close();
    return false;
}

void MainPass::handleInstructions(std::list<NodePtr> &constraint_list,
                                  Instruction &instruction, bool &isPureFunc, std::string FuncName) {
    auto opcode = instruction.getOpcode();
    switch (opcode) {
        // Terminator instructions
        case Instruction::Ret: {
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
            std::list<NodePtr> udexpr = this->newHandleFunctionCall(*func, true);
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
                std::cout<<"before filling:\n";
                printConstraints(constraint_list, instruction);
                Node::fillUndeterminedPredicate(udexpr, constraint_list, &instruction);
                constraint_list = udexpr;
                std::cout<<"after filling:\n";
                printConstraints(constraint_list, instruction);
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
