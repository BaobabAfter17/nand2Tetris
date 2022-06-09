#include <string>
#include <unordered_map>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <iostream>

using namespace std;

void parse_line_symbol(string filename, unordered_map<string, int> &symbol_table)
{

  string in_filename = filename + ".o1";
  ifstream infile(in_filename);
  string out_filename = filename + ".o2";
  ofstream outfile(out_filename);

  string line;
  int line_count{0};
  while (getline(infile, line)) {
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());

    if (line[0] == '(') {   // (LOOP)
      string line_name = line.substr(1, line.size() - 2);
      symbol_table[line_name] = line_count;
      continue;
    }
    outfile << line << "\n";
    line_count++;
  }

  infile.close();
  outfile.close();
}

void parse_variables(string filename, unordered_map<string, int> &symbol_table)
{
  string in_filename = filename + ".o2";
  ifstream infile(in_filename);

  string line;
  int var_addr{16};   // variable address start from 16
  while (getline(infile, line)) {
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());

    if (line[0] == '@' && !isdigit(line[1])) {  // @foo
      string var_name = line.substr(1);
      if (symbol_table.find(var_name) == symbol_table.end())
        symbol_table[var_name] = var_addr++;
    }
  }

  infile.close();
}

unordered_map<string, int> parse_symbols(const string &filename)
{
  unordered_map<string, int> symbol_table = {
    {"SP", 0},
    {"LCL", 1},
    {"ARG", 2},
    {"THIS", 3},
    {"THAT", 4},
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    {"R8", 8},
    {"R9", 9},
    {"R10", 10},
    {"R11", 11},
    {"R12", 12},
    {"R13", 13},
    {"R14", 14},
    {"R15", 15},
    {"SCREEN", 16384},
    {"KBD", 24576},
  };

  parse_line_symbol(filename, symbol_table);
  parse_variables(filename, symbol_table);
  return move(symbol_table);
}