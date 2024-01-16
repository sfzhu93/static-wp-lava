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
    WpPrinter(std::string fileName);
    WpPrinter();
    WpPrinter(const WpPrinter& other) = delete; // Prevent copying
    WpPrinter(WpPrinter&& other) noexcept;      // Move constructor
    WpPrinter& operator=(const WpPrinter& other) = delete; // Prevent copy assignment
    WpPrinter& operator=(WpPrinter&& other) noexcept;      // Move assignment
    ~WpPrinter();

    void open(const std::string &filename);
    void emit(const std::string &llvmIns, const std::string &Wp);
    void setBlockName(const std::string &blockname);
    void setFuncName(const std::string &funcname);
    void close();

private:
    std::string fileName;
    std::string blockName;
    std::string funcName;

    std::ofstream fout;
    bool hasStarted = false;
};

#endif //STATIC_WP_LAVA_WPPRINTER_H
