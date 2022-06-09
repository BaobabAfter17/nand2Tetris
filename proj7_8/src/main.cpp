#include <string>
#include <fstream>
#include <filesystem>

#include "constants.h"
#include "parser.h"
#include "code_writer.h"

using namespace std;

void TranslateSingleFile(const string vm_pathname, CodeWriter & cw) {
  Parser p { vm_pathname };
  while (p.HasMoreLines()) {
    p.Advance();
    switch (p.CurrentCommandType()) {
    case CommandType::kCArithmetic:
      cw.WriteArithmetic(p.Arg1());
      break;
    case CommandType::kCPush:
    case CommandType::kCPop:
      cw.WritePushPop(p.CurrentCommandType(), p.Arg1(), p.Arg2());
      break;
    case CommandType::kCLabel:
      cw.WriteLabel(p.Arg1());
      break;
    case CommandType::kCGoto:
      cw.WriteGoto(p.Arg1());
      break;
    case CommandType::kCIf:
      cw.WriteIf(p.Arg1());
      break;
    case CommandType::kCFunction:
      cw.WriteFunction(p.Arg1(), p.Arg2());
      break;
    case CommandType::kCCall:
      cw.WriteCall(p.Arg1(), p.Arg2());
      break;
    case CommandType::kCReturn:
      cw.WriteReturn();
      break;
    default:
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  filesystem::path const directory_path { argv[1] };
  string directory_name = directory_path.filename().string();
  auto asm_path = directory_path / filesystem::path(directory_name + ".asm");
  CodeWriter cw{ asm_path.string() };
  cw.WriteBootstrapCodes();

  // Translate each .vm file
  for (const auto &entry : filesystem::directory_iterator(directory_path)) {
    if (entry.is_regular_file()
        && entry.path().extension().string() == ".vm") {
      const auto vm_filename = entry.path().filename().string();
      cw.SetFileName(vm_filename);
      const auto vm_pathname = entry.path().string();
      TranslateSingleFile(vm_pathname, cw);
    }
  }

  cw.WriteInfiniteLoop();
  return 0;
}