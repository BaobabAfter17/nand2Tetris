#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

// Takes only one argument (`filename`) as input. Output <filename>.out
// at same directory as <filename>.in. `filename` could be absolute or
// relative directory. The output file remove all whitespace and comments
// from <filename>.in.
void remove_comment(const string &in_filename);

unordered_map<string, int> parse_symbols(const string &filename);

void assemble(const unordered_map<string, int> & symbol_table, const string &filename);