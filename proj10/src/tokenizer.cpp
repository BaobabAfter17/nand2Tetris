#include "tokenizer.h"
#include "constants.h"

#include <sstream>
#include <string>
#include <cassert>
#include <cctype>

using namespace std;

Tokenizer::Tokenizer(const string &jack_filename)
  : input_jack_file(jack_filename), line(),
    char_index(), current_token(), in_multiline_comments(false) {
}

// Try to get the next token. If success, put the token back and return true
// else return false.
bool Tokenizer::HasMoreTokens() {
  while (!CurrentLineHasMoreTokens()) {
    ReadNextLineFromFile();
    char_index = 0; // reset index to start
    if (input_jack_file.eof()) break; // No more lines in file
  }

  return CurrentLineHasMoreTokens();
}

void Tokenizer::Advance() {
  if (!HasMoreTokens()) return;
  while (char_index < line.length() && isspace(line[char_index])) {
    char_index++;
  }
  stringstream ss{};
  while (char_index < line.length() && !isspace(line[char_index])) {
    ss << line[char_index];
    char_index++;
  }
  current_token = ss.str();
}

TokenType Tokenizer::CurrentTokenType() {
  assert(!current_token.empty());
  // keyword
  for (auto keyword : kAllKeywords) {
    if (current_token == keyword) { return kKeyword; }
  }
  // symbol
  if (kAllSymbols.find(current_token) != string::npos) {
    return kSymbol;
  }
  // integerConstant
  if (isdigit(current_token[0])) {
    return kIntegerConstant;
  }
  // stringConstant
  if (current_token[0] == '"') {
    return kStringConstant;
  }
  // default identifier
  return kIdentifier;
}

const string Tokenizer::Keyword() {
  assert(CurrentTokenType() == kKeyword);
  return current_token;
}

char Tokenizer::Symbol() {
  assert(CurrentTokenType() == kSymbol);
  return current_token[0];  // current_token shall be a string with single char
}

string Tokenizer::Identifier() {
  assert(CurrentTokenType() == kIdentifier);
  return current_token;
}

int Tokenizer::IntVal() {
  assert(CurrentTokenType() == kIntegerConstant);
  return stoi(current_token);
}

// Now current_token starts with a double quote, could be 1) ",  2) "foo,
// 3) "foo". Anyway, read line_ss until the second double quote.
string Tokenizer::StringVal() {
  assert(CurrentTokenType() == kStringConstant);
  string quoted_string;
  // current_token starts and ends with double quote
  if (current_token.length() > 1
      && current_token[current_token.length() - 1] == '"') {
    quoted_string = current_token;

  // otherwise need to read until the closing double quote
  } else {
    stringstream ss{};
    ss << current_token;
    while (char_index < line.length() && line[char_index] != '"') {
      ss << line[char_index++];
    }
    ss << line[char_index++]; // add the closing double quote
    quoted_string = ss.str();
  }
  
  return quoted_string.substr(1, quoted_string.length() - 2);
}

///////////////////////// Private Methods//////////////////////////////////////

bool Tokenizer::CurrentLineHasMoreTokens() {
  for (int i = char_index; i < line.length(); i++) {
    if (!isspace(line[i]))
      return true;
  }
  return false;
}

// Read next line
void Tokenizer::ReadNextLineFromFile() {
  do {
    ReadLineAndCheckComment();
  } while (in_multiline_comments);
  RemoveCommentsFromLine();
  PadSymbolsWithWhitespace();
}

void Tokenizer::ReadLineAndCheckComment() {
  getline(input_jack_file, line);
  if (line.find("/**") != string::npos && !in_multiline_comments) {
    in_multiline_comments = true;
  }
  if (line.rfind("*/") != string::npos && in_multiline_comments) {
    in_multiline_comments = false;
  }
}

void Tokenizer::RemoveCommentsFromLine() {
  // remove inline comment // ...
  auto comment_pos = line.find("//");
  if (comment_pos != string::npos) {
    line = line.substr(0, comment_pos);
  }

  // remove /** ... */
  auto comment_start = line.find("/**");
  auto comment_end = line.rfind("*/");
  if (comment_start == string::npos && comment_end == string::npos) return;
  
  if (comment_start == string::npos) comment_start = 0;
  if (comment_end == string::npos) comment_end = line.length() - 2;
  // "hello/*some comment*/world" -> "helloworld"
  line.erase(comment_start, comment_end + 1 - comment_start + 1);
}

void Tokenizer::PadSymbolsWithWhitespace() {
  stringstream ss;
  for (auto ch : line) {
    if (kAllSymbols.find(ch) != string::npos) { // ch is a symbol
      ss << ' ' << ch << ' '; // pad whitspaces on both sides
    } else {
      ss << ch;
    }
  }
  line = ss.str();
}