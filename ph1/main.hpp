#ifndef MAIN_H
#define MAIN_H

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
enum GateType
{
    INPUT,
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    XNOR,
    INVERTER,
    BUFFER
};
struct Node
{
    std::vector< size_t > input;
    size_t                output;
    std::string           outputName;
    GateType              gateType;
    bool                  isWellDefined = false;
};

auto getFromBracket( std::string st ) -> std::string;
auto filterSpace( std::string st ) -> std::string;
void addInput( std::string st );
void addOutput( std::string st );
void addGate( std::string st );
auto getGateType( std::string st ) -> GateType;
auto getFromComma( std::string st ) -> std::vector< std::string >;
void doSort( );
void eval( std::string );
void print( );
#endif // MAIN_H