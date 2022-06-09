#ifndef COMMAND_TYPE_H
#   define COMMAND_TYPE_H

#include <vector>
#include <string>

using std::vector;
using std::string;

enum class CommandType {
  kCArithmetic,
  kCPush,
  kCPop,
  kCLabel,
  kCGoto,
  kCIf,
  kCFunction,
  kCCall,
  kCReturn,
  kCUnknown,
};

const string
  kComment = "//",
  kSP = "SP",
  kLCL = "LCL",
  kARG = "ARG",
  kTHIS = "THIS",
  kTHAT = "THAT",
  kLocal = "local",
  kArgument = "argument",
  kThis = "this",
  kThat = "that",
  kStatic = "static",
  kTemp = "temp",
  kPointer = "pointer",
  kConstant = "constant";

const string
  kAdd = "add",
  kSub = "sub",
  kNeg = "neg",
  kEq = "eq",
  kGt = "gt",
  kLt = "lt",
  kAnd = "and",
  kOr = "or",
  kNot = "not",
  kPush = "push",
  kPop = "pop",
  kLabel = "label",
  kGoto = "goto",
  kIf = "if-goto",
  kFunction = "function",
  kCall = "call",
  kReturn = "return";

const vector<string> kBinaryCodes =
  {
    "// <add/sub/and/or>",
    "@SP",
    "AM=M-1",
    "D=M",
    "A=A-1",
    "<M=D+M/M-D/D&M/D|M>",
  };

const vector<string> kUnaryCodes =
  {
    "// <neg/not>",
    "@SP",
    "A=M-1",
    "<M=-M/!M>",
  };


const vector<string> kCompareCodes =
  {
    "// <eq/gt/lt>",
    "@SP",
    "AM=M-1",
    "D=M",
    "A=A-1",
    "D=M-D",
    "M=-1",
    "@CONTINUE<i>",
    "D;<JEQ/JLT/JGT>",
    "@SP",
    "A=M-1",
    "M=0",
    "(CONTINUE<i>)",
  };

const vector<string> kPushConstantCodes =
  {
    "// push constant <index>",
    "@<index>",
    "D=A",
    "@SP",
    "AM=M+1",
    "A=A-1",
    "M=D",
  };

const vector<string> kPushCodes =
  {
    "// push <segment> <index>",
    "@<segment>",
    "D=M",
    "@<index>",
    "A=D+A",
    "D=M",
    "@SP",
    "A=M",
    "M=D",
    "@SP",
    "M=M+1",
  };

const vector<string> kPushCodes1 =
  {
    "// push <temp/static/pointer> <index>",
    "@<index+5/index+16/THIS/THAT>",
    "D=M",
    "@SP",
    "A=M",
    "M=D",
    "@SP",
    "M=M+1",
  };

const vector<string> kPopCodes =
  {
    "// pop <segment> <index>",
    "@<segment>",
    "D=M",
    "@<index>",
    "D=D+A",
    "@R13",
    "M=D",
    "@SP",
    "AM=M-1",
    "D=M",
    "@R13",
    "A=M",
    "M=D",
  };

const vector<string> kPopCodes1 =
  {
    "// pop <temp/static/pointer> <index>",
    "@SP",
    "AM=M-1",
    "D=M",
    "@<index+5/index+16/THIS/THAT>",
    "M=D",
  };

const vector<string> kLabelCodes =
  {
    "// label <label>",
    "(<function$label>)",
  };

const vector<string> kGotoCodes =
  {
    "// goto <label>",
    "@<function$label>",
    "0;JMP",
  };

const vector<string> kIfCodes =
  {
    "// if-goto <label>",
    "@SP",
    "AM=M-1",
    "D=M",
    "@<function$label>",
    "D;JNE",
  };

const vector<string> kFuncCodes = {
  "// function <function_name> <n_vars>",
  "(<function_name>)",
};

const vector<string> kInitLocalCodes = {
  "@SP",
  "AM=M+1",
  "A=A-1",
  "M=0",
};

const vector<string> kInfiniteLoopCodes =
  {
    "// Infinite Loop",
    "(INFINITE_LOOP)",
    "@INFINITE_LOOP",
    "0;JMP",
  };

const vector<string> kReturnCodes =
  {
    "@LCL",     // FRAME = LCL
    "D=M",
    "@FRAME",
    "M=D",
    "@FRAME",   // RET = *(FRAME - 5)
    "D=M",
    "@5",
    "A=D-A",
    "D=M",
    "@RET",
    "M=D",
    "@SP",      // *ARG = pop()
    "AM=M-1",
    "D=M",
    "@ARG",
    "A=M",
    "M=D",
    "@ARG",     // SP = ARG + 1
    "D=M",
    "@SP",
    "M=D+1",
    "@FRAME",   // THAT = *(FRAME - 1)
    "A=M-1",
    "D=M",
    "@THAT",
    "M=D",
    "@FRAME",   // THIS = *(FRAME - 2)
    "D=M-1",
    "A=D-1",
    "D=M",
    "@THIS",
    "M=D",
    "@FRAME",   // ARG = *(FRAME - 3)
    "D=M-1",
    "D=D-1",
    "A=D-1",
    "D=M",
    "@ARG",
    "M=D",
    "@FRAME",   // LCL = *(FRAME - 4)
    "D=M",
    "@4",
    "A=D-A",
    "D=M",
    "@LCL",
    "M=D",
    "@RET",     // goto RET
    "A=M",
    "0;JMP",
  };

const vector<string> kCallCodes =
  {
    /* Do the following in advance
    * 1) add comment as "// call <f> <n>"
    * 2) push return address
    * 3) push LCL
    * 4) push ARG
    * 5) push THIS
    * 6) push THAT
    */

    "@SP",                  // ARG = SP - n - 5
    "D=M",
    "@<n+5>",               // 2
    "D=D-A",
    "@ARG",
    "M=D",
    "@SP",                  // LCL = SP
    "D=M",
    "@LCL",
    "M=D",
    "@<function_name>",     // 10 goto <function_name>
    "0;JMP",
    "(<return_address>)",   // 12 label return address
  };

const vector<string> kInitSPCodes =
 {
   "@256",
   "D=A",
   "@SP",
   "M=D",
 };

#endif