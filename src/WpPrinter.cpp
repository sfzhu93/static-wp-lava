//
// Created by zsf on 19-3-27.
//

#include "WpPrinter.h"

WpPrinter::WpPrinter(std::string fileName) : fileName(std::move(fileName)), fout(this->fileName) {}

WpPrinter::WpPrinter() = default;

WpPrinter::WpPrinter(WpPrinter&& other) noexcept
    : fileName(std::move(other.fileName)),
      blockName(std::move(other.blockName)),
      funcName(std::move(other.funcName)),
      fout(std::move(other.fout)),
      hasStarted(other.hasStarted) {
    other.hasStarted = false;
}

WpPrinter& WpPrinter::operator=(WpPrinter&& other) noexcept {
    if (this != &other) {
        fileName = std::move(other.fileName);
        blockName = std::move(other.blockName);
        funcName = std::move(other.funcName);
        fout = std::move(other.fout);
        hasStarted = other.hasStarted;
        other.hasStarted = false;
    }
    return *this;
}

WpPrinter::~WpPrinter() {
    close();
}

void WpPrinter::open(const std::string &filename) {
    fileName = filename;
    fout.open(fileName);
}

void WpPrinter::emit(const std::string &llvmIns, const std::string &Wp) {
    if (!hasStarted) {
        hasStarted = true;
        fout << "# In block " << blockName << ", function " << funcName << std::endl;
        fout << "| LLVM Instruction | Precondition |" << std::endl;
        fout << "|-----|-----|" << std::endl;
    }
    fout << "| " << llvmIns << " | " << Wp << " |" << std::endl;
}

void WpPrinter::setBlockName(const std::string &blockname) {
    blockName = blockname;
    hasStarted = false;
}

void WpPrinter::setFuncName(const std::string &funcname) {
    funcName = funcname;
    hasStarted = false;
}

void WpPrinter::close() {
    if (fout.is_open()) {
        llvm::outs() << "wpprinter close";
        fout.close();
    }
}
