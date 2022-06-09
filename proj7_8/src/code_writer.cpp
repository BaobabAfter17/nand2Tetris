#include <string>
#include <map>
#include <vector>

#include "code_writer.h"
#include "constants.h"

using namespace std;

void CodeWriter::SetFileName(string filename) {
  vm_file_name = filename;
}

void CodeWriter::SetFunctionName(string function) {
  current_function = function;
}

void CodeWriter::WriteArithmetic(string command) {
  if (command == kEq || command == kGt || command == kLt) {
    WriteCodesToFile(GenerateCompareCodes(command));
  } else if (command == kAdd || command == kSub || command == kAnd || command == kOr) {
    WriteCodesToFile(GenerateBinaryCodes(command));
  } else if (command == kNeg || command == kNot) {
    WriteCodesToFile(GenerateUnaryCodes(command));
  } else {
    throw runtime_error("unknow arithmetic command");
  }
}

void CodeWriter::WritePushPop(CommandType cmd_type, string segment, int index) {
  if (segment == kConstant) {
    WriteCodesToFile(GeneratePushConstantCodes(index));
  } else if (segment == kLocal || segment == kArgument || segment == kThis || segment == kThat) {
    WriteCodesToFile(GeneratePushPopCodes(cmd_type, segment, index));
  } else if (segment == kTemp || segment == kStatic || segment == kPointer) {
    WriteCodesToFile(GeneratePushPopCodes1(cmd_type, segment, index));
  } else {
    throw runtime_error("unknow push or pop command");
  }
}

void CodeWriter::WriteLabel(string label) {
  auto codes = kLabelCodes;
  codes[0] = "// label " + label;
  codes[1] = "(" + current_function + "$" + label + ")";
  WriteCodesToFile(codes);
}

void CodeWriter::WriteGoto(string label) {
  auto codes = kGotoCodes;
  codes[0] = "// goto " + label;
  codes[1] = "@" + current_function + "$" + label;
  WriteCodesToFile(codes);
}

void CodeWriter::WriteIf(string label) {
  auto codes = kIfCodes;
  codes[0] = "// if-goto " + label;
  codes[4] = "@" + current_function + "$" + label;
  WriteCodesToFile(codes);
}

void CodeWriter::WriteFunction(string function_name, int n_vars) {
  SetFunctionName(function_name);
  auto codes = kFuncCodes;
  codes[0] = "// function " + function_name + " " + to_string(n_vars);
  codes[1] = "(" + function_name + ")";
  WriteCodesToFile(codes);

  for (int i = 0; i < n_vars; i++) {
    WriteCodesToFile(kInitLocalCodes);
  }
}

void CodeWriter::WriteReturn() {
  WriteCodesToFile(kReturnCodes);
}

void CodeWriter::WriteCall(string function_name, int n_vars) {
  // write comment
  string comment = "// call " + function_name + " " + to_string(n_vars);
  vector<string> codes = { comment };
  WriteCodesToFile(codes);

  // push return_address/LCL/ARG/THIS/THAT
  string return_address = function_name + GetUniqueMarker();
  const vector<string> symbols = {
    return_address, kLCL, kARG, kTHIS, kTHAT,
  };
  for (auto const &sym : symbols) {
    codes = kPushConstantCodes;
    codes[0] = "// push " + sym;
    codes[1] = "@" + sym;
    codes[2] = sym == return_address ? "D=A" : "D=M";
    WriteCodesToFile(codes);
  }

  // Remaining codes
  codes = kCallCodes;
  codes[2] = "@" + to_string(n_vars + 5);
  codes[10] = "@" + function_name;
  codes[12] = "(" + return_address + ")";
  WriteCodesToFile(codes);
}

void CodeWriter::WriteInfiniteLoop() {
  WriteCodesToFile(kInfiniteLoopCodes);
}

void CodeWriter::WriteBootstrapCodes() {
  WriteCodesToFile(kInitSPCodes);
  WriteCall("Sys.init", 0);
}

vector<string> CodeWriter::GenerateCompareCodes(const string command) {
  vector<string> codes = { kCompareCodes };
  codes[0] = "// " + command;

  auto marker = GetUniqueMarker();
  codes[7] = "@CONTINUE" + marker;
  codes[12] = "(CONTINUE" + marker + ")";

  static const map<string, string> cmd_asms {
    {kEq, "JEQ"},
    {kGt, "JGT"},
    {kLt, "JLT"},
  };
  codes[8] = "D;" + cmd_asms.at(command);
  return codes;
}

vector<string> CodeWriter::GenerateBinaryCodes(const string command) {
  vector<string> codes = { kBinaryCodes };
  codes[0] = "// " + command;

  static const map<string, string> cmd_asms{
    {kAdd, "D+M"},
    {kSub, "M-D"},
    {kAnd, "D&M"},
    {kOr, "D|M"},
  };
  codes[5] = "M=" + cmd_asms.at(command);
  return codes;
}

vector<string> CodeWriter::GenerateUnaryCodes(const string command) {
  vector<string> codes = { kUnaryCodes };
  codes[0] = "// " + command;
  codes[3] = command == kNeg ? "M=-M" : "M=!M";
  return codes;
}

vector<string> CodeWriter::GeneratePushConstantCodes(int index) {
  auto codes = kPushConstantCodes;
  codes[0] = "// push constant " + to_string(index);
  codes[1] = "@" + to_string(index);
  return codes;
}

vector<string> CodeWriter::GeneratePushPopCodes(CommandType cmd_type, string segment, int index) {
  const map<string, string> segments_registers {
    {kLocal, kLCL},
    {kArgument, kARG},
    {kThis, kTHIS},
    {kThat, kTHAT},
  };

  auto codes = cmd_type == CommandType::kCPush ? kPushCodes : kPopCodes;
  auto command_str = cmd_type == CommandType::kCPush ? kPush : kPop;
  codes[0] = "// " + command_str + " " + segment + " " + to_string(index);
  codes[1] = "@" + segments_registers.at(segment);
  codes[3] = "@" + to_string(index);
  return codes;
}

vector<string> CodeWriter::GeneratePushPopCodes1(CommandType cmd_type, string segment, int index) {
  const map<string, string> segments_registers {
    {kPointer, index == 0 ? kTHIS : kTHAT},
    {kTemp, to_string(index + 5)},
    {kStatic, vm_file_name + to_string(index + 16)},
  };

  auto codes = cmd_type == CommandType::kCPush ? kPushCodes1 : kPopCodes1;
  auto command_str = cmd_type == CommandType::kCPush ? kPush : kPop;
  codes[0] = "// " + command_str + " " + segment + " " + to_string(index);
  int line_index = cmd_type == CommandType::kCPush ? 1 : 4;
  codes[line_index] = "@" + segments_registers.at(segment);
  return codes;
}

void CodeWriter::WriteCodesToFile(const vector<string> codes) {
  for (auto code : codes) {
    asm_file << code << "\n";
  }
}

string CodeWriter::GetUniqueMarker() {
  return to_string(unique_marker++);
}