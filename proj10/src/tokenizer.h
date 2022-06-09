/* Ignores all comments and white space in the input stream and eanbles
 * accessing the input one token at a time. Also parses the provides the
 * type of each token, as defined by the Jack grammer */

#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include <fstream>
#include <sstream>
#include <string>

#include "constants.h"

using std::ifstream;
using std::istringstream;
using std::string;
using TokenType = const string;

class Tokenizer {
 public:
  // Opens the input .jack file and gets ready to tokenize it
  Tokenizer(const string & jack_filename);

  // Are there more tokens in the input?
  bool HasMoreTokens();

  // Get the next token from the input, and makes it the current token. This
  // method should be called only if HasMoreTokens is true. Initially there
  // is no current token.
  void Advance();

  // Returns the type of the current token, as a constant
  TokenType CurrentTokenType();

  // Returns the keyword shich is the current token, as a constant. This method
  // should only be called only if CurrentTokenType is kKeyword Type.
  const string Keyword();

  // Returns the character which is the current token. Should be called only if
  // CurrentTokenType is kSymbol Type.
  char Symbol();

  // Returns the string which is the current token. Should be called only if
  // CurrentTokenType is kIdentifier Type
  string Identifier();

  // Returns the integer value of the current token. Should be called only if
  // CurrentTokenType is kIntConst Type.
  int IntVal();

  // Returns the string value of the current token, whitout the opening and
  // closing double quotes. Should be called only if CurrentTokenType is
  // kStringConst.
  string StringVal();

 private:
  ifstream input_jack_file;
  string line;  // current line read from input_jack_file
  bool in_multiline_comments; // current line in multiple line comments?
  int char_index;
  string current_token;  // updated when calling Advance()

  bool CurrentLineHasMoreTokens();

  // Extract next line from input_jack_file, remove comment, pad symbols with
  // whitspace and replace the content of line_ss with new line.
  void ReadNextLineFromFile();
  
  // Put token back to line_ss. Does not check whether token is
  // recently extracted from line_ss.
  void PutTokenBackToLine(const string &token);

  // Read one line from input_jack_file, update in_multiline_comments status.
  void ReadLineAndCheckComment();

  // Remove comments (/* ... */ and // ... ) from string line, in place.
  void RemoveCommentsFromLine();

  // Pad symbols in the line, as listed in constants.h, with whitspaces on both
  // sides. This is done in place.
  // example: "x=y+1;" -> "x = y + 1 ; "
  void PadSymbolsWithWhitespace();
};

#endif