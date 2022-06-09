#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

#include "parser.h"
#include "constants.h"

bool Parser::HasMoreLines() {
  return vm_file.peek() != EOF;
}

void Parser::Advance() {
  line.clear();
  while (HasMoreLines()) {
    getline(vm_file, line);
    TrimLine();
    if (!line.empty())
      break;
  }
}

string Parser::GetCommandHeader() {
  auto first_space_pos = line.find(" ");
  first_space_pos =
    first_space_pos == string::npos ? line.length() : first_space_pos;
  return line.substr(0, first_space_pos);
}

CommandType Parser::CurrentCommandType() {
  static const map<string, CommandType> headers_cmds = {
    {kAdd, CommandType::kCArithmetic},
    {kSub, CommandType::kCArithmetic},
    {kNeg, CommandType::kCArithmetic},
    {kEq, CommandType::kCArithmetic},
    {kGt, CommandType::kCArithmetic},
    {kLt, CommandType::kCArithmetic},
    {kAnd, CommandType::kCArithmetic},
    {kOr, CommandType::kCArithmetic},
    {kNot, CommandType::kCArithmetic},
    {kPush, CommandType::kCPush},
    {kPop, CommandType::kCPop},
    {kLabel, CommandType::kCLabel},
    {kGoto, CommandType::kCGoto},
    {kIf, CommandType::kCIf},
    {kFunction, CommandType::kCFunction},
    {kCall, CommandType::kCCall},
    {kReturn, CommandType::kCReturn},
  };

  auto header = GetCommandHeader();
  return headers_cmds.at(header);
}

string Parser::Arg1() {
  auto first_space_pos = line.find(" ");
  first_space_pos =
    first_space_pos == string::npos ? -1 : first_space_pos;
  auto second_space_pos = line.rfind(" ");
  second_space_pos
    = second_space_pos == string::npos ? line.length() : second_space_pos;
  return line.substr(first_space_pos + 1,
                     second_space_pos - first_space_pos - 1);
}

int Parser::Arg2() {
  auto second_space_pos = line.rfind(" ");
  auto index_s = line.substr(second_space_pos + 1);
  return stoi(index_s);
}

void Parser::TrimLine() {
  // remove comment
  auto comment_pos = line.find(kComment);
  if (comment_pos != string::npos) {
    line = line.substr(0, comment_pos);
  }

  // remove trailing whitespace
  const string kWhiteSpace = " \t\f\v\n\r";
  auto end_pos = line.find_last_not_of(kWhiteSpace);
  end_pos = end_pos == string::npos ? -1 : end_pos;
  line = line.substr(0, end_pos + 1);
}
