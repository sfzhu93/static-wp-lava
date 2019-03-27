//
// Created by zsf on 19-3-27.
//

#ifndef STATIC_WP_LAVA_WPPRINTER_H
#define STATIC_WP_LAVA_WPPRINTER_H

#include <string>
#include <fstream>
#include "llvm/Support/raw_ostream.h"

class WpPrinter {
public:
    std::string fileName;
    std::ofstream fout;
    bool hasStarted = false;

    WpPrinter(std::string fileName) {
        this->fileName = std::move(fileName);
        fout.open(this->fileName);
    }

    WpPrinter() {}

    void open(const std::string &filename) {
        this->fileName = std::move(fileName);
        fout.open(this->fileName);
    }

    void emit(const std::string &llvmIns, const std::string &Wp){
        if (!hasStarted){
            hasStarted = true;
            fout<<"| LLVM Instruction | Precondition |"<<std::endl;
            fout<<"|-----|-----|"<<std::endl;
        }
        fout<<"| "<<llvmIns<<" | "<<Wp<<" |"<<std::endl;
    }

    void close() {
        llvm::outs()<<"wpprinter close";
        fout.close();
    }

    ~WpPrinter(){
        fout.close();
    }

};


#endif //STATIC_WP_LAVA_WPPRINTER_H
