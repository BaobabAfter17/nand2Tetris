/* In syntax analyzer (project 10), CompilationEngine emits a structured
 * representation of the input source code wrapped in XML tags. In the compiler
 * the CompilationEngine will isntead emit the executable VM code. In both
 * versions, the parsing logic are the same as following:
 * 
 * The CompilationEngine gets ints input from Tokenizer and emits its output to
 * an output file.*/

#ifndef JACK_COMPILATION_ENGINE_H
#define JACK_COMPILATION_ENGINE_H

#include <string>
#include <fstream>

#include "tokenizer.h"

using std::string;
using std::ofstream;

class CompilationEngine {
 public:
  // Creates a new compilation engine with the given input and output. The next
  // routeine called (by the Analyzer module) must be CompileClass.
  CompilationEngine(const string &input_filename, const string &output_filename);
  
  // Compile a complete class.
  void CompileClass();

  // Compile a static variable declaration, or a filed declaration.
  void CompileClassVarDec();

  // Compile a complete method, function, or constructor
  void CompileSubroutineDec();

  // Compile a (possibley empty) parameter list. Does not handle the enclosing
  // parenthesese token ( and ).
  void CompileParameterList();

  // Complile a subroutine body.
  void CompileSubsroutineBody();

  // Compiles a var declaration.
  void CompileVarDec();

  // Compile a sequence of statements. Doe not handle the enclosing curly
  // bracket { and }.
  void CompileStatements();

  // Compile a let statment
  void CompileLet();

  // Compile an if statement, possibly with a trailing else statement.
  void CompileIf();

  // Compile a while statment.
  void CompileWhile();

  // Compile a do statement.
  void CompileDo();

  // Compile a return statement.
  void CompileReturn();

  // Compile an expression.
  void CompileExpression();

  // Compile a term. If the current tokne is an identifier, the routine must
  // resolve it into a variable, and array element, or a subroutine call. A
  // single lookahead token, which may be [, (, or . suffices to distinguish
  // between the possibilities. Any other token is not part of this term and 
  // should not be advanced over.
  void CompileTerm();

  // Compile a (possibly empty) comma-separated list of expressions. Returns
  // the number of expressions in the list.
  int CompileExpressionList();
 
 private:
  ofstream output_xml_file;
  Tokenizer tokenizer;

  // Assert current token of tokenizer is keyword and output it to xml_file.
  // Advance tokenizer.
  void ProcessKeyword(string keyword);

  // Assert current token of tokenizer is an identifier and output it to
  // xml_file. Advance tokenizer.
  void ProcessIdentifier();

  // Assert current token of tokenizer is symbol and output to xml_file.
  // Advance tokenizer.
  void ProcessSymbol(char symbol);

  // Assert current token is integer constant and output to xml_file.
  // Advance tokenizer.
  void ProcessIntegerConstant();

  // Assert current token is string constant and output to xml_file.
  // Advance tokenizer.
  void ProcessStringConstant();

  // Convert special symbols <>"& to XML style and keep other sybols the same.
  string ConvertToXMLSymbol(char symbol);

  // Compile <type>, <void | type>.
  // No <type> or </type> added to xml_file.
  void CompileTypeHelper();

  // + - * / & | < > =
  bool IsOperator(char symbol);

  // true, false, null, this
  bool IsKeyWordConstant(string keyword);

  // ~ -
  bool IsUnaryOprator(char symbol);
};

#endif
