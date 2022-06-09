#include "symbol_table.h"
#include "constants.h"

#include <string>
#include <stdexcept>
#include <tuple>

using namespace std;

SymbolTable::SymbolTable() : table(), var_counts() {
}

void SymbolTable::Reset() {
  table.clear();
  var_counts.clear();
}

void SymbolTable::Define(const string name, const string type, VarKind kind) {
  if (table.find(name) != table.end())
    throw runtime_error(name + " is already in symbol table!");
  
  auto index = var_counts[kind]++;
  table[name] = Entry(type, kind, index);
}

int SymbolTable::VarCount(VarKind kind) {
  return var_counts[kind];
}

VarKind SymbolTable::KindOf(const string name) {
  if (table.find(name) == table.end())
    return kNoneKind;
  return get<VarKind>(table.at(name));
}

string SymbolTable::TypeOf(const string name) {
  return get<string>(table.at(name));
}

int SymbolTable::IndexOf(const string name) {
  return get<int>(table.at(name));
}