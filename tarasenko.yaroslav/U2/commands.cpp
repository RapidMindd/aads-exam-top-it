#include "commands.hpp"

#include <fstream>
#include <string>

#include "parse_utils.hpp"
#include "queries.hpp"

namespace
{
  const char INVALID_COMMAND[] = "<INVALID COMMAND>";

  std::string readCommandName(const std::string& line, std::size_t& position)
  {
    position = tarasenko::skipSpaces(line, position);
    const std::size_t first = position;
    while ((position < line.size()) && !tarasenko::isSpace(line[position])) {
      ++position;
    }
    return line.substr(first, position - first);
  }

  bool readExistingId(const std::string& line,
      std::size_t& position,
      const tarasenko::Database& database,
      std::size_t& id)
  {
    if (!tarasenko::readSize(line, position, id)) {
      return false;
    }
    return tarasenko::hasPerson(database, id);
  }

  bool runAnons(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    if (!tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    tarasenko::writeAnons(output, database);
    return true;
  }

  bool runDesc(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    std::size_t id = 0;
    if (!readExistingId(line, position, database, id) || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::writeDescription(output, database, id);
  }

  bool runRedesc(const std::string& line,
      std::size_t position,
      tarasenko::Database& database)
  {
    std::size_t id = 0;
    std::string info;
    if (!tarasenko::readSize(line, position, id)) {
      return false;
    }
    if (!tarasenko::readQuotedText(line, position, info) || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    tarasenko::setPersonInfo(database, id, info);
    return true;
  }

  bool runDeanon(const std::string& line,
      std::size_t position,
      tarasenko::Database& database)
  {
    std::size_t anonId = 0;
    std::size_t id = 0;
    if (!tarasenko::readSize(line, position, anonId) || !tarasenko::readSize(line, position, id)
        || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::mergeAnonPerson(database, anonId, id);
  }

  bool runMeets(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    std::size_t id = 0;
    if (!readExistingId(line, position, database, id) || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::writeMeetings(output, database, id);
  }

  bool runCommons(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    std::size_t first = 0;
    std::size_t second = 0;
    if (!readExistingId(line, position, database, first)
        || !readExistingId(line, position, database, second)
        || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::writeCommonPersons(output, database, first, second);
  }

  bool runLess(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!tarasenko::readSize(line, position, time)
        || !readExistingId(line, position, database, id)
        || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::writeLessMeetings(output, database, time, id);
  }

  bool runGreater(const std::string& line,
      std::size_t position,
      std::ostream& output,
      const tarasenko::Database& database)
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!tarasenko::readSize(line, position, time)
        || !readExistingId(line, position, database, id)
        || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }
    return tarasenko::writeGreaterMeetings(output, database, time, id);
  }

  bool runOutPersons(const std::string& line,
      std::size_t position,
      const tarasenko::Database& database)
  {
    position = tarasenko::skipSpaces(line, position);
    const std::size_t first = position;
    while ((position < line.size()) && !tarasenko::isSpace(line[position])) {
      ++position;
    }
    if ((first == position) || !tarasenko::isOnlySpaces(line, position)) {
      return false;
    }

    const std::string filename = line.substr(first, position - first);
    std::ofstream output(filename.c_str());
    if (!output.is_open()) {
      return false;
    }
    tarasenko::writePersonsWithInfo(output, database);
    return static_cast< bool >(output);
  }

  bool processCommandLine(const std::string& line,
      std::ostream& output,
      tarasenko::Database& database)
  {
    std::size_t position = 0;
    const std::string command = readCommandName(line, position);
    if (command == "anons") {
      return runAnons(line, position, output, database);
    }
    if (command == "deanon") {
      return runDeanon(line, position, database);
    }
    if (command == "redesc") {
      return runRedesc(line, position, database);
    }
    if (command == "desc") {
      return runDesc(line, position, output, database);
    }
    if (command == "meets") {
      return runMeets(line, position, output, database);
    }
    if (command == "commons") {
      return runCommons(line, position, output, database);
    }
    if (command == "less") {
      return runLess(line, position, output, database);
    }
    if (command == "greater") {
      return runGreater(line, position, output, database);
    }
    if (command == "out-persons") {
      return runOutPersons(line, position, database);
    }
    return false;
  }
}

void tarasenko::processCommands(std::istream& input, std::ostream& output, Database& database)
{
  std::string line;
  while (std::getline(input, line)) {
    if (!processCommandLine(line, output, database)) {
      output << INVALID_COMMAND << '\n';
    }
  }
}

