#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include "constants.h"

using std::ofstream;
using std::string;
using std::vector;

class CodeWriter {
 public:
  CodeWriter(const string filename)
    : asm_file(filename), unique_marker(0), vm_file_name(""), current_function("") {};
  void SetFileName(string filename);
  void SetFunctionName(string function);
  void WriteArithmetic(string command);
  void WritePushPop(CommandType command, string segment, int index);
  void WriteLabel(string label);
  void WriteGoto(string label);
  void WriteIf(string label);
  void WriteFunction(string function_name, int n_vars);
  void WriteReturn();
  void WriteCall(string function_name, int n_vars);
  void WriteInfiniteLoop();
  void WriteBootstrapCodes();

 private:
  ofstream asm_file;
  int unique_marker;
  string vm_file_name;
  string current_function;

  string GetUniqueMarker();
  void WriteCodesToFile(const vector<string> codes);
  vector<string> GenerateCompareCodes(const string command);
  vector<string> GenerateBinaryCodes(const string command);
  vector<string> GenerateUnaryCodes(const string command);
  vector<string> GeneratePushConstantCodes(int index);
  vector<string> GeneratePushPopCodes(CommandType cmd_type, string segment, int index);
  vector<string> GeneratePushPopCodes1(CommandType cmd_type, string segment, int index);
};

#endif