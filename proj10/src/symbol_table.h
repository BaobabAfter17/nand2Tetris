/* Provides services for building, populating, ans using symbo tables that keep
 * track of the symbol properties name, type, kind and a running index */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <tuple>

#include "constants.h"

using std::string;
using std::unordered_map;
using std::tuple;
using Entry = tuple<string, VarKind, int>;   // <type, kind, index>

class SymbolTable {
 public:
  // Creates a SymbolTable.
  SymbolTable();

  // Empties the SymbolTable, and resets the four indices to 0. Should be
  // called when starting to compile a subroutine declaration.
  void Reset();

  // Adds to the table a new variable of the given name, type, kind. Assign to
  // it the index value of that kind, and adds 1 to the index.
  void Define(const string name, const string type, VarKind kind);

  // Return the number of variables of the given kind already defined in the
  // table.
  int VarCount(VarKind kind);

  // Return the kind of the named identifier. If not found, return kNoneKind
  VarKind KindOf(const string name);

  // Return the type of the varaible.
  string TypeOf(const string name);

  // Retun the index of the varaible.
  int IndexOf(const string name);


 private:
  // An unorderd_map to store SymbolTable entries, with names as keys and its
  // (type, kind, index) as values.
  unordered_map<string, Entry> table;

  // A unordered_map to record number of stored variables for each VarKind. 
  unordered_map<VarKind, int> var_counts;

};

#endif