/* Constants and Enums */
#ifndef JACK_COMPILER_CONSTANTS_H
#define JACK_COMPILER_CONSTANTS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// Token types
const string
  kKeyword = "keyword",
  kSymbol = "symbol",
  kIntegerConstant = "integerConstant",
  kStringConstant = "stringConstant",
  kIdentifier = "identifier";

// Keywords
const string
  kClass = "class",
  kMethod = "method",
  kFunction = "function",
  kConstructor = "constructor",
  kInt = "int",
  kBoolean = "boolean",
  kChar = "char",
  kVoid = "void",
  kVar = "var",
  kStatic = "static",
  kField = "field",
  kLet = "let",
  kDo = "do",
  kIf = "if",
  kElse = "else",
  kWhile = "while",
  kReturn = "return",
  kTrue = "true",
  kFalse = "false",
  kNull = "null",
  kThis = "this";

const vector<string> kAllKeywords = {
    kClass, kMethod, kFunction, kConstructor,
    kInt, kBoolean, kChar, kVoid,
    kVar, kStatic, kField,
    kLet, kDo, kIf, kElse, kWhile, kReturn,
    kTrue, kFalse, kNull, kThis,
};

// Escape sequences for some symbols
const string
  kLt = "&lt;",  // <
  kGt = "&gt;",  // >
  kQuote = "&quot;", // "
  kAmp = "&amp;";  // &

// '{' '}' '(' ')' '[' ']' '.' ',' ';' '+' '-' '*' '/' '&' '|' '<' '>' '=' '~'
const string kAllSymbols = "{}()[].,;+-*/&|<>=~";

// The kind of an identifier
enum VarKind {
  kStaticKind = 0,
  kFieldKind,
  kArgKind,
  kAvarKind,
  kNoneKind
};

#endif