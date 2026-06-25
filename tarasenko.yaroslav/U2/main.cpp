#include <fstream>
#include <iostream>
#include <string>

#include "commands.hpp"
#include "domain.hpp"
#include "loaders.hpp"

namespace tarasenko
{
  namespace
  {
  const int SUCCESS = 0;
  const int INVALID_ARGUMENTS = 1;
  const int FILE_OPEN_ERROR = 2;
  const int DATA_ERROR = 3;
  const char INPUT_PREFIX[] = "in:";
  const char DATA_PREFIX[] = "data:";

  struct ProgramOptions
  {
    bool hasInput;
    bool hasData;
    std::string inputName;
    std::string dataName;
  };

  bool startsWith(const std::string& text, const char* prefix)
  {
    const std::string prefixText(prefix);
    return (text.size() >= prefixText.size())
        && (text.compare(0, prefixText.size(), prefixText) == 0);
  }

  bool setInput(ProgramOptions& options, const std::string& argument)
  {
    const std::string prefix(INPUT_PREFIX);
    if (options.hasInput || (argument.size() == prefix.size())) {
      return false;
    }
    options.hasInput = true;
    options.inputName = argument.substr(prefix.size());
    return true;
  }

  bool setData(ProgramOptions& options, const std::string& argument)
  {
    const std::string prefix(DATA_PREFIX);
    if (options.hasData || (argument.size() == prefix.size())) {
      return false;
    }
    options.hasData = true;
    options.dataName = argument.substr(prefix.size());
    return true;
  }

  bool parseArguments(int argc, char* argv[], ProgramOptions& options)
  {
    const int minArgumentCount = 2;
    const int maxArgumentCount = 3;
    if ((argc < minArgumentCount) || (argc > maxArgumentCount)) {
      return false;
    }

    options = { false, false, "", "" };
    for (int index = 1; index < argc; ++index) {
      const std::string argument(argv[index]);
      if (startsWith(argument, INPUT_PREFIX)) {
        if (!setInput(options, argument)) {
          return false;
        }
      } else if (startsWith(argument, DATA_PREFIX)) {
        if (!setData(options, argument)) {
          return false;
        }
      } else {
        return false;
      }
    }

    return options.hasData;
  }

  bool hasTooManyArguments(int argc)
  {
    const int maxArgumentCount = 3;
    return argc > maxArgumentCount;
  }

  bool loadInputFile(const ProgramOptions& options, tarasenko::Database& database)
  {
    if (!options.hasInput) {
      return true;
    }

    std::ifstream input(options.inputName.c_str());
    if (!input.is_open()) {
      return false;
    }
    tarasenko::loadPersons(input, database);
    return true;
  }

  int loadDataFile(const ProgramOptions& options, tarasenko::Database& database)
  {
    std::ifstream input(options.dataName.c_str());
    if (!input.is_open()) {
      return FILE_OPEN_ERROR;
    }
    if (!tarasenko::loadMeetings(input, database)) {
      return DATA_ERROR;
    }
    return SUCCESS;
  }
  }
}

int main(int argc, char* argv[])
{
  if (tarasenko::hasTooManyArguments(argc)) {
    std::cerr << "Too many arguments\n";
    return tarasenko::SUCCESS;
  }

  tarasenko::ProgramOptions options = { false, false, "", "" };
  if (!tarasenko::parseArguments(argc, argv, options)) {
    std::cerr << "Invalid command line arguments\n";
    return tarasenko::INVALID_ARGUMENTS;
  }

  tarasenko::Database database = tarasenko::makeDatabase();
  if (!tarasenko::loadInputFile(options, database)) {
    std::cerr << "Cannot open input file\n";
    tarasenko::destroyDatabase(database);
    return tarasenko::FILE_OPEN_ERROR;
  }

  const int dataStatus = tarasenko::loadDataFile(options, database);
  if (dataStatus == tarasenko::FILE_OPEN_ERROR) {
    std::cerr << "Cannot open data file\n";
    tarasenko::destroyDatabase(database);
    return tarasenko::FILE_OPEN_ERROR;
  }
  if (dataStatus == tarasenko::DATA_ERROR) {
    std::cerr << "Invalid meetings data\n";
    tarasenko::destroyDatabase(database);
    return tarasenko::DATA_ERROR;
  }

  tarasenko::processCommands(std::cin, std::cout, database);
  tarasenko::destroyDatabase(database);
  return tarasenko::SUCCESS;
}
