/* The main program that drives the overall syntax analysis process, using the
 * services of a Tokenizer and a CompilationEngine. For each source .jack file
 * the analyzer
 * 
 * 1. creates a Tokenizer from .jack input file.
 * 2. creates an output file named .xml; and
 * 3. use the Tokenizer and the CompilationEngine to parse the input file and
 * write the parsed code to the output file. */

#include "compilation_engine.h"
#include "tokenizer.h"
#include "constants.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <map>

using namespace std;
namespace fs = std::filesystem;

void ParseSingleJackFile(const fs::path & jack_path) {
  auto xml_path = jack_path;
  xml_path.replace_extension(".xml");
  CompilationEngine ce{ jack_path, xml_path };
  ce.CompileClass();
}

int main(int argc, char* argv[])
{
  fs::path input_path( argv[1] );
  // single .jack file
  if (input_path.extension() == ".jack") {
    ParseSingleJackFile(input_path);

  // a directory contains multiple .jack file
  } else {
    for (const auto &entry : fs::directory_iterator(input_path)) {
      if (entry.is_regular_file() && entry.path().extension() == ".jack") {
        ParseSingleJackFile(entry);
      }
    }
  }
}