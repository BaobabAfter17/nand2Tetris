#include <string>
#include <fstream>
#include "assembler.h"

using namespace std;


int main(int argc, char *argv[])
{
  string in_filename = argv[1];
  string filename = in_filename.substr(0, in_filename.find_last_of('.')); // remove ".asm"
  remove_comment(filename);
  auto symbol_table = parse_symbols(filename);
  assemble(symbol_table, filename);
  return 0;
}