#ifndef PARSER_H
#   define PARSER_H

#include <string>
#include <fstream>
#include "constants.h"

using std::string;
using std::ifstream;

class Parser {
 public:
  Parser(const string filename) : vm_file(filename) {};
  bool HasMoreLines();
  void Advance();
  CommandType CurrentCommandType();
  string Arg1();
  int Arg2();

 private:
  ifstream vm_file; // RAII, no need to close
  string line;
  string GetCommandHeader();
  void TrimLine();
};

#endif