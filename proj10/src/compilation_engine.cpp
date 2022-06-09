#include "compilation_engine.h"
#include "tokenizer.h"
#include "constants.h"

#include <cassert>
#include <map>

using namespace std;

CompilationEngine::CompilationEngine(const string &input_filename,
                                     const string &output_filename)
  : output_xml_file(output_filename), tokenizer(input_filename) {
}

void CompilationEngine::CompileClass() {
  output_xml_file << "<class>\n";
  tokenizer.Advance();  // fetch first token, which should be keyword "class"
  ProcessKeyword(kClass);
  ProcessIdentifier();
  ProcessSymbol('{');

  // Potentially 0 or more ClassVarDec or Subroutine, all start with keyword
  while (tokenizer.CurrentTokenType() == kKeyword
         && (tokenizer.Keyword() == kStatic
             || tokenizer.Keyword() == kField
             || tokenizer.Keyword() == kConstructor
             || tokenizer.Keyword() == kFunction
             || tokenizer.Keyword() == kMethod)) {
    if (tokenizer.Keyword() == kStatic || tokenizer.Keyword() == kField) {
      CompileClassVarDec();
    } else if (tokenizer.Keyword() == kConstructor
           || tokenizer.Keyword() == kFunction
           || tokenizer.Keyword() == kMethod) {
      CompileSubroutineDec();
    }
  }
  
  ProcessSymbol('}');
  output_xml_file << "</class>\n";
}

void CompilationEngine::CompileClassVarDec() {
  output_xml_file << "<classVarDec>\n";
  ProcessKeyword(tokenizer.Keyword());  // static or field
  CompileTypeHelper();
  ProcessIdentifier();  // varName

  // (, varName)*
  while (tokenizer.CurrentTokenType() == kSymbol
         && tokenizer.Symbol() == ',') {
    ProcessSymbol(',');
    ProcessIdentifier();  // varName
  }
  ProcessSymbol(';');
  output_xml_file << "</classVarDec>\n";
}

void CompilationEngine::CompileSubroutineDec() {
  output_xml_file << "<subroutineDec>\n";
  ProcessKeyword(tokenizer.Keyword());  // consturctor | function | method
  CompileTypeHelper();
  ProcessIdentifier();  // subroutineName
  ProcessSymbol('(');
  CompileParameterList();
  ProcessSymbol(')');
  CompileSubsroutineBody();
  output_xml_file << "</subroutineDec>\n";
}

void CompilationEngine::CompileParameterList() {
  output_xml_file << "<parameterList>\n";
  if (tokenizer.CurrentTokenType() != kKeyword  // no ParameterList
      && tokenizer.CurrentTokenType() != kIdentifier) {
    output_xml_file << "</parameterList>\n";
    return;
  }
  CompileTypeHelper();  // type
  ProcessIdentifier();  // varName

  // (, type varName)*
  while (tokenizer.CurrentTokenType() == kSymbol
         && tokenizer.Symbol() == ',') {
    ProcessSymbol(',');
    CompileTypeHelper();  // type
    ProcessIdentifier();  // varName
  }
  output_xml_file << "</parameterList>\n";
}

void CompilationEngine::CompileSubsroutineBody() {
  output_xml_file << "<subroutineBody>\n";
  ProcessSymbol('{');
  // varDec*
  while (tokenizer.CurrentTokenType() == kKeyword
         && tokenizer.Keyword() == kVar) {
    CompileVarDec();
  }
  CompileStatements();
  ProcessSymbol('}');
  output_xml_file << "</subroutineBody>\n";
}

void CompilationEngine::CompileVarDec() {
  output_xml_file << "<varDec>\n";
  ProcessKeyword(kVar); // var
  CompileTypeHelper();  // type
  ProcessIdentifier();  // varName
  // (, varName)*
  while (tokenizer.CurrentTokenType() == kSymbol
         && tokenizer.Symbol() == ',') {
    ProcessSymbol(',');
    ProcessIdentifier();
  }
  ProcessSymbol(';');
  output_xml_file << "</varDec>\n";
}

void CompilationEngine::CompileStatements() {
  output_xml_file << "<statements>\n";
  while (tokenizer.CurrentTokenType() == kKeyword
         && (tokenizer.Keyword() == kLet || tokenizer.Keyword() == kIf
             || tokenizer.Keyword() == kWhile || tokenizer.Keyword() == kDo
             || tokenizer.Keyword() == kReturn)) {
    if (tokenizer.Keyword() == kIf) CompileIf();
    else if (tokenizer.Keyword() == kLet) CompileLet();
    else if (tokenizer.Keyword() == kWhile) CompileWhile();
    else if (tokenizer.Keyword() == kDo) CompileDo();
    else if (tokenizer.Keyword() == kReturn) CompileReturn();
  }
  output_xml_file << "</statements>\n";
}

void CompilationEngine::CompileLet() {
  output_xml_file << "<letStatement>\n";
  ProcessKeyword(kLet); // let
  ProcessIdentifier();  // varName
  // ([ expression ])?
  if (tokenizer.CurrentTokenType() == kSymbol && tokenizer.Symbol() == '[') {
    ProcessSymbol('[');
    CompileExpression();
    ProcessSymbol(']');
  }
  ProcessSymbol('=');
  CompileExpression();
  ProcessSymbol(';');
  output_xml_file << "</letStatement>\n";
}

void CompilationEngine::CompileIf() {
  output_xml_file << "<ifStatement>\n";
  ProcessKeyword(kIf);
  ProcessSymbol('(');
  CompileExpression();
  ProcessSymbol(')');
  ProcessSymbol('{');
  CompileStatements();
  ProcessSymbol('}');
  // (else { statements })?
  if (tokenizer.CurrentTokenType() == kKeyword
      && tokenizer.Keyword() == kElse) {
    ProcessKeyword(kElse);
    ProcessSymbol('{');
    CompileStatements();
    ProcessSymbol('}');
  }
  output_xml_file << "</ifStatement>\n";
}

void CompilationEngine::CompileWhile() {
  output_xml_file << "<whileStatement>\n";
  ProcessKeyword(kWhile);
  ProcessSymbol('(');
  CompileExpression();
  ProcessSymbol(')');
  ProcessSymbol('{');
  CompileStatements();
  ProcessSymbol('}');
  output_xml_file << "</whileStatement>\n";
}

void CompilationEngine::CompileDo() {
  output_xml_file << "<doStatement>\n";
  ProcessKeyword(kDo);

  // subroutineCall
  ProcessIdentifier();
  // . subroutineName
  if (tokenizer.CurrentTokenType() == kSymbol && tokenizer.Symbol() == '.') {
    ProcessSymbol('.');
    ProcessIdentifier();
  }
  // ( parameterList )
  ProcessSymbol('(');
  CompileExpressionList();
  ProcessSymbol(')');

  ProcessSymbol(';');
  output_xml_file << "</doStatement>\n";
}

void CompilationEngine::CompileReturn() {
  output_xml_file << "<returnStatement>\n";
  ProcessKeyword(kReturn);
  // (expression)?
  if (!(tokenizer.CurrentTokenType() == kSymbol
        && tokenizer.Symbol() == ';')) {
    CompileExpression();
  }
  ProcessSymbol(';');
  output_xml_file << "</returnStatement>\n";
}

void CompilationEngine::CompileExpression() {
  output_xml_file << "<expression>\n";
  CompileTerm();
  // (op term)*
  while (tokenizer.CurrentTokenType() == kSymbol
         && IsOperator(tokenizer.Symbol())) {
    ProcessSymbol(tokenizer.Symbol());
    CompileTerm();
  }
  output_xml_file << "</expression>\n";
}

void CompilationEngine::CompileTerm() {
  output_xml_file << "<term>\n";
  // integerConstant
  if (tokenizer.CurrentTokenType() == kIntegerConstant) {
    ProcessIntegerConstant();
  // stringConstant
  } else if (tokenizer.CurrentTokenType() == kStringConstant) {
    ProcessStringConstant();
  // keywordConstant
  } else if (tokenizer.CurrentTokenType() == kKeyword
             && IsKeyWordConstant(tokenizer.Keyword())) {
    ProcessKeyword(tokenizer.Keyword());
  // ( expression )
  } else if (tokenizer.CurrentTokenType() == kSymbol
             && tokenizer.Symbol() == '(') {
    ProcessSymbol('(');
    CompileExpression();
    ProcessSymbol(')');
  } else if (tokenizer.CurrentTokenType() == kSymbol
             && IsUnaryOprator(tokenizer.Symbol())) {
    ProcessSymbol(tokenizer.Symbol());
    CompileTerm();
  // varName | varName, [ expression ] | subroutineCall
  } else if (tokenizer.CurrentTokenType() == kIdentifier) {
    ProcessIdentifier();  // varName | subroutineName
    // [ expression ]
    if (tokenizer.CurrentTokenType() == kSymbol && tokenizer.Symbol() == '[') {
      ProcessSymbol('[');
      CompileExpression();
      ProcessSymbol(']');
    // subroutineCall (w/ subroutineName processed)
    } else if (tokenizer.CurrentTokenType() == kSymbol
               && (tokenizer.Symbol() == '(' || tokenizer.Symbol() == '.')) {
      // . subroutineName
      if (tokenizer.CurrentTokenType() == kSymbol && tokenizer.Symbol() == '.') {
        ProcessSymbol('.');
        ProcessIdentifier();
      }
      // ( parameterList )
      ProcessSymbol('(');
      CompileExpressionList();
      ProcessSymbol(')');
    }
  }
  output_xml_file << "</term>\n";
}

int CompilationEngine::CompileExpressionList() {
  output_xml_file << "<expressionList>\n";
  // Coulde be empty
  // notice expressinList is always followed by a ')' in the Jack grammar
  if (tokenizer.CurrentTokenType() == kSymbol && tokenizer.Symbol() == ')') {
    output_xml_file << "</expressionList>\n";
    return 0;
  }

  int num_expressions = 0;
  // expression
  CompileExpression();
  num_expressions++;
  // (, expression)*
  while (tokenizer.CurrentTokenType() == kSymbol
         && tokenizer.Symbol() == ',') {
    ProcessSymbol(',');
    CompileExpression();
    num_expressions++;
  }
  output_xml_file << "</expressionList>\n";
  return num_expressions;
}

///////////////////Private Methods/////////////////////

void CompilationEngine::ProcessKeyword(string keyword) {
  assert(tokenizer.Keyword() == keyword);
  output_xml_file << "<" << tokenizer.CurrentTokenType() << "> ";
  output_xml_file << tokenizer.Keyword();
  output_xml_file << " </" << tokenizer.CurrentTokenType() << ">\n";
  tokenizer.Advance();
}

void CompilationEngine::ProcessIdentifier() {
  assert(tokenizer.CurrentTokenType() == kIdentifier);
  output_xml_file << "<" << tokenizer.CurrentTokenType() << "> ";
  output_xml_file << tokenizer.Identifier();
  output_xml_file << " </" << tokenizer.CurrentTokenType() << ">\n";
  tokenizer.Advance();
}

void CompilationEngine::ProcessSymbol(char symbol) {
  assert(tokenizer.Symbol() == symbol);
  output_xml_file << "<" << tokenizer.CurrentTokenType() << "> ";
  output_xml_file << ConvertToXMLSymbol(tokenizer.Symbol());
  output_xml_file << " </" << tokenizer.CurrentTokenType() << ">\n";
  tokenizer.Advance();
}

void CompilationEngine::ProcessIntegerConstant() {
  assert(tokenizer.CurrentTokenType() == kIntegerConstant);
  output_xml_file << "<" << tokenizer.CurrentTokenType() << "> ";
  output_xml_file << tokenizer.IntVal();
  output_xml_file << " </" << tokenizer.CurrentTokenType() << ">\n";
  tokenizer.Advance();
}

void CompilationEngine::ProcessStringConstant() {
  assert(tokenizer.CurrentTokenType() == kStringConstant);
  output_xml_file << "<" << tokenizer.CurrentTokenType() << "> ";
  output_xml_file << tokenizer.StringVal();
  output_xml_file << " </" << tokenizer.CurrentTokenType() << ">\n";
  tokenizer.Advance();
}

string CompilationEngine::ConvertToXMLSymbol(char symbol) {
  static map<char, const string> special_symbols {
    {'>', kGt },
    {'<', kLt },
    {'"', kQuote },
    {'&', kAmp }
  };
  if (special_symbols.find(symbol) != special_symbols.end()) {
    return special_symbols.at(symbol);
  }
  return string(1, symbol);
}

void CompilationEngine::CompileTypeHelper() {
  // void | int | char | boolean
  if (tokenizer.CurrentTokenType() == kKeyword
     && (tokenizer.Keyword() == kVoid || tokenizer.Keyword() == kInt
        || tokenizer.Keyword() == kChar || tokenizer.Keyword() == kBoolean)) {
    ProcessKeyword(tokenizer.Keyword());
  } else {
    ProcessIdentifier();  // className
  }
}

bool CompilationEngine::IsOperator(char symbol) {
  static string kAllOps = "+-*/&|<>=";
  return kAllOps.find(symbol) != string::npos;
}

bool CompilationEngine::IsKeyWordConstant(string keyword) {
  return keyword == kTrue || keyword == kFalse
         || keyword == kNull || keyword == kThis;
}

bool CompilationEngine::IsUnaryOprator(char symbol) {
  return symbol == '-' || symbol == '~';
}