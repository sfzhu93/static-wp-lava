//
// Created by zsf on 19-3-27.
//

#ifndef STATIC_WP_LAVA_WPPRINTER_H
#define STATIC_WP_LAVA_WPPRINTER_H

#include <string>
#include <fstream>
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <unistd.h>

class WpPrinter {
public:
    std::string fileName;
    std::string blockName;
    std::string funcName;

    std::ofstream fout;
    bool hasStarted = false;
    FILE *fp;

    WpPrinter(std::string fileName) {
        this->fileName = std::move(fileName);
        fout.open(this->fileName);
    }

    WpPrinter() {}

    void open(const std::string &filename) {
        this->fileName = filename;
        /*char pwd[1024];
        getcwd(pwd, sizeof(pwd));

        this->fp = fopen("/home/suz305/a.out", "w+");//fopen((filename+"_c_api").c_str(), "w+");
        fprintf(fp, "%s", pwd);
        fclose(fp);*/
        fout.open(this->fileName);
    }

    void emit(const std::string &llvmIns, const std::string &Wp){
        if (!hasStarted){
            hasStarted = true;
            fout<<"# In block "<<this->blockName<<", function "<<this->funcName<<std::endl;
            fout<<"| LLVM Instruction | Precondition |"<<std::endl;
            fout<<"|-----|-----|"<<std::endl;
        }
        fout<<"| "<<llvmIns<<" | "<<Wp<<" |"<<std::endl;
    }

    /*void emit_blockname(const std::string &name) {
        hasStarted = false;
        fout<<"# In block: "<<name<<std::endl;
    }*/

    void setBlockName(const std::string &blockname) {
        this->blockName = blockname;
        hasStarted = false;
    }

    void setFuncName(const std::string &funcname) {
        this->funcName = funcname;
        hasStarted = false;
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
