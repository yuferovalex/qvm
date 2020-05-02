#ifndef VM_PSEUDOASSEMBLERLANGUAGE_H
#define VM_PSEUDOASSEMBLERLANGUAGE_H

#include "qvm/Command.h"

#define QVM_ASM_BEGIN(type) []() { auto program = type; Command command;

#define UNARY_CMD_HELPER_(cmd, l, lref, res)                              \
    command.operation = cmd | ((lref) * (Command::Flags::LHS_OP_REF);     \
    command.lhsOperand = l;                                               \
    command.resultRef = res;                                              \
    program.push_back(command)

#define BINARY_CMD_HELPER_(cmd, l, lref, r, rref, res)                                                           \
    command.operation = cmd | ((lref) * Command::Flags::LHS_OP_REF) | ((rref) * Command::Flags::RHS_OP_REF);     \
    command.lhsOperand = l;                                                                                      \
    command.rhsOperand = r;                                                                                      \
    command.resultRef = res;                                                                                     \
    program.push_back(command)

#define VAL(v)  Command::Operand(Command::Value( v )), 0
#define REF(r)  Command::Operand(Command::Reference( r )), 1

#define CMD_ADD(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::ADD, l, r, res)
#define CMD_SUB(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::SUB, l, r, res)
#define CMD_MUL(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::MUL, l, r, res)
#define CMD_DIV(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::DIV, l, r, res)

#define CMD_ABS(l, res)     UNARY_CMD_HELPER_(Command::Operation::ABS , l, res)
#define CMD_SQRT(l, res)    UNARY_CMD_HELPER_(Command::Operation::SQRT, l, res)
#define CMD_NEG(l, res)     UNARY_CMD_HELPER_(Command::Operation::NEG , l, res)
#define CMD_NOT(l, res)     UNARY_CMD_HELPER_(Command::Operation::NOT , l, res)

#define CMD_OR(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::OR  , l, r, res)
#define CMD_AND(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::AND , l, r, res)
#define CMD_XOR(l, r, res)  BINARY_CMD_HELPER_(Command::Operation::XOR , l, r, res)

#define CMD_EQ(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::EQ  , l, r, res)
#define CMD_NE(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::NE  , l, r, res)
#define CMD_LT(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::LT  , l, r, res)
#define CMD_LE(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::LE  , l, r, res)
#define CMD_GT(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::GT  , l, r, res)
#define CMD_GE(l, r, res)   BINARY_CMD_HELPER_(Command::Operation::GE  , l, r, res)

#define QVM_ASM_END   return program; } ()

#endif //VM_PSEUDOASSEMBLERLANGUAGE_H
