#include <fstream>
#include <iostream>
#include <string>

#include "input_processing.hpp"
#include "person_output.hpp"
#include "person_storage.hpp"

namespace
{
  const int SUCCESS = 0;
  const int INVALID_ARGUMENTS = 1;
  const int FILE_OPEN_ERROR = 2;
  const char INPUT_PREFIX[] = "in:";
  const char OUTPUT_PREFIX[] = "out:";

  struct ProgramOptions
  {
    bool hasInput;
    bool hasOutput;
    std::string inputName;
    std::string outputName;
  };

  bool startsWith(const std::string& text, const char* prefix)
  {
    const std::string prefixText(prefix);
    return (text.size() >= prefixText.size())
        && (text.compare(0, prefixText.size(), prefixText) == 0);
  }

  bool setInputOption(ProgramOptions& options, const std::string& argument)
  {
    const std::string prefix(INPUT_PREFIX);
    if (options.hasInput || (argument.size() == prefix.size())) {
      return false;
    }

    options.hasInput = true;
    options.inputName = argument.substr(prefix.size());
    return true;
  }

  bool setOutputOption(ProgramOptions& options, const std::string& argument)
  {
    const std::string prefix(OUTPUT_PREFIX);
    if (options.hasOutput || (argument.size() == prefix.size())) {
      return false;
    }

    options.hasOutput = true;
    options.outputName = argument.substr(prefix.size());
    return true;
  }

  bool parseArguments(int argc, char* argv[], ProgramOptions& options)
  {
    const int maxArgumentCount = 3;
    if (argc > maxArgumentCount) {
      return false;
    }

    options = { false, false, "", "" };
    for (int index = 1; index < argc; ++index) {
      const std::string argument(argv[index]);
      if (startsWith(argument, INPUT_PREFIX)) {
        if (!setInputOption(options, argument)) {
          return false;
        }
      } else if (startsWith(argument, OUTPUT_PREFIX)) {
        if (!setOutputOption(options, argument)) {
          return false;
        }
      } else {
        return false;
      }
    }

    return true;
  }

  bool readInput(const ProgramOptions& options,
      tarasenko::PersonStorage& storage,
      tarasenko::ReadStats& stats)
  {
    if (!options.hasInput) {
      stats = tarasenko::readPersons(std::cin, storage);
      return true;
    }

    std::ifstream input(options.inputName.c_str());
    if (!input.is_open()) {
      return false;
    }

    stats = tarasenko::readPersons(input, storage);
    return true;
  }

  bool writeOutput(const ProgramOptions& options, const tarasenko::PersonStorage& storage)
  {
    if (!options.hasOutput) {
      tarasenko::writePersons(std::cout, storage);
      return static_cast< bool >(std::cout);
    }

    std::ofstream output(options.outputName.c_str());
    if (!output.is_open()) {
      return false;
    }

    tarasenko::writePersons(output, storage);
    return static_cast< bool >(output);
  }
}

int main(int argc, char* argv[])
{
  ProgramOptions options = { false, false, "", "" };
  if (!parseArguments(argc, argv, options)) {
    return INVALID_ARGUMENTS;
  }

  tarasenko::PersonStorage storage = tarasenko::makePersonStorage();
  tarasenko::ReadStats stats = { 0, 0 };
  if (!readInput(options, storage, stats)) {
    tarasenko::destroyPersonStorage(storage);
    return FILE_OPEN_ERROR;
  }

  if (!writeOutput(options, storage)) {
    tarasenko::destroyPersonStorage(storage);
    return FILE_OPEN_ERROR;
  }

  std::cerr << stats.accepted << ' ' << stats.ignored << '\n';
  tarasenko::destroyPersonStorage(storage);
  return SUCCESS;
}
