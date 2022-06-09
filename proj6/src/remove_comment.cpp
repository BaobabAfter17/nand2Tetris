#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

void remove_enters(string & line)
{
  line.erase(remove(line.begin(), line.end(), '\n'), line.end());
  line.erase(remove(line.begin(), line.end(), '\r'), line.end());
}

void remove_spaces(string &line)
{
  line.erase(remove(line.begin(), line.end(), ' '), line.end());
  line.erase(remove(line.begin(), line.end(), '\t'), line.end());
}

void remove_inline_comments(string &line)
{
  auto search = line.find("//");
  if (search != string::npos) {
    line.erase(search);
  }
}

void remove_multiline_comments(string &line, bool &in_comment)
{
  auto start = line.find("/*");
  auto end = line.find("*/");
  if (start == string::npos && end == string::npos)
    return;
  if (start != string::npos && end == string::npos) { // 1) /*...
    line.erase(start);
    in_comment = true;
    return;
  }
  if (start == string::npos && end != string::npos) { // 2) ...*/
    line.erase(0, end + 2);
    in_comment = false;
    return;
  }
  if (start != string::npos && end != string::npos && start + 1 < end) { // 3) /*...*/
    line.erase(start, end + 2);
    return;
  }
  if (start != string::npos && end != string::npos && end + 1 < start) {  // 4) ...*/code/*...
    line.erase(0, end + 2);
    line.erase(start);
    return;
  }
  throw runtime_error("Incorrect comment!");
}

void remove_comment(const string & filename)
{
  string in_filename = filename + ".asm";
  ifstream infile(in_filename);
  string out_filename = filename + ".o1";
  ofstream outfile(out_filename);

  string line;
  bool in_comment = false;
  while (getline(infile, line)) {
    if (in_comment)
      continue;
    remove_enters(line);
    remove_spaces(line);
    remove_inline_comments(line);
    remove_multiline_comments(line, in_comment);
    if (line.size() > 1)
      outfile << line << "\n";
  }

  infile.close();
  outfile.close();
}