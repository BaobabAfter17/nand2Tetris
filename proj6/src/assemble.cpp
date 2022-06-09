#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset>
#include <iostream>
#include <algorithm>

using namespace std;

string parse_A_instruction(const unordered_map<string, int> &symbol_table, const string &line)
{
  const string value_str = line.substr(1);
  int value = symbol_table.find(value_str) != symbol_table.end()
      ? symbol_table.at(value_str) // value_str in symbol_table
      : stoi(value_str);          // value_str is a value
  return "0" + bitset<15>(value).to_string() + "\n";
}

string parse_header(const string &comp)
{
  if (comp.find("M") != string::npos) // use M
    return "1111";
  else
    return "1110";
}

string parse_dest(const string &dest)
{
  static unordered_map<string, string> dests_map = {
    {"", "000"},
    {"M", "001"},
    {"D", "010"},
    {"DM", "011"},
    {"MD", "011"},
    {"A", "100"},
    {"AM", "101"},
    {"AD", "110"},
    {"ADM", "111"},
  };
  if (dests_map.find(dest) != dests_map.end())
    return dests_map[dest];
  else
    return "invalid dest";
}

string parse_comp(const string &comp)
{
  static unordered_map<string, string> comps_map = {
    {"0", "101010"},
    {"1", "111111"},
    {"-1", "111010"},
    {"D", "001100"},
    {"A", "110000"},
    {"M", "110000"},
    {"!D", "001101"},
    {"!A", "110001"},
    {"!M", "110001"},
    {"-D", "001111"},
    {"-A", "110011"},
    {"-M", "110011"},
    {"D+1", "011111"},
    {"A+1", "110111"},
    {"M+1", "110111"},
    {"D-1", "001110"},
    {"A-1", "110010"},
    {"M-1", "110010"},
    {"D+A", "000010"},
    {"D+M", "000010"},
    {"M+D", "000010"},
    {"D-A", "010011"},
    {"D-M", "010011"},
    {"A-D", "000111"},
    {"M-D", "000111"},
    {"D&A", "000000"},
    {"D&M", "000000"},
    {"D|A", "010101"},
    {"D|M", "010101"},
  };
  if (comps_map.find(comp) != comps_map.end())
    return comps_map[comp];
  else
    return "invalid comp";
}

string parse_jmp(const string &jmp)
{
  static unordered_map<string, string> jmps_map = {
    {"", "000"},
    {"JGT", "001"},
    {"JEQ", "010"},
    {"JGE", "011"},
    {"JLT", "100"},
    {"JNE", "101"},
    {"JLE", "110"},
    {"JMP", "111"},
  };
  if (jmps_map.find(jmp) != jmps_map.end())
    return jmps_map[jmp];
  else
    return "invalid jmp";
}

string parse_C_instruction(const string &line)
{
  auto equal_pos = line.find("=");
  string dest = equal_pos != string::npos ? line.substr(0, equal_pos) : "";
  auto semicolon_pos = line.find(";");
  string jmp = semicolon_pos != string::npos ? line.substr(semicolon_pos + 1) : "";
  auto comp_start = equal_pos != string::npos ? equal_pos+1 : 0;
  string comp = line.substr(comp_start, semicolon_pos);

  return parse_header(comp) + parse_comp(comp) + parse_dest(dest) + parse_jmp(jmp) + "\n";
}


void assemble(const unordered_map<string, int> &symbol_table, const string &filename)
{
  string in_filename = filename + ".o2";
  ifstream infile(in_filename);
  string out_filename = filename + ".hack";
  ofstream outfile(out_filename);

  string line;
  while (getline(infile, line)) {
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());

    if (line[0] == '@') {
      outfile << parse_A_instruction(symbol_table, line);
    } else {
      outfile << parse_C_instruction(line);
    }
  }

  infile.close();
  outfile.close();
}