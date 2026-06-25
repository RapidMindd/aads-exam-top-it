#include "input_processing.hh"

#include <cctype>
#include <istream>
#include <limits>

namespace
{
  bool isSpace(char symbol)
  {
    return std::isspace(static_cast< unsigned char >(symbol)) != 0;
  }

  std::size_t skipSpaces(const std::string& line, std::size_t position)
  {
    while ((position < line.size()) && isSpace(line[position])) {
      ++position;
    }
    return position;
  }

  bool readId(const std::string& line, std::size_t& position, std::size_t& id)
  {
    position = skipSpaces(line, position);
    if ((position == line.size()) || !std::isdigit(static_cast< unsigned char >(line[position]))) {
      return false;
    }

    const std::size_t maxSize = std::numeric_limits< std::size_t >::max();
    std::size_t parsedId = 0;
    while ((position < line.size()) && std::isdigit(static_cast< unsigned char >(line[position]))) {
      const std::size_t digit = static_cast< std::size_t >(line[position] - '0');
      if (parsedId > ((maxSize - digit) / 10)) {
        return false;
      }
      parsedId = (parsedId * 10) + digit;
      ++position;
    }

    id = parsedId;
    return true;
  }
}

bool tarasenko::parsePersonLine(const std::string& line, Person& person)
{
  std::size_t position = 0;
  std::size_t id = 0;
  if (!readId(line, position, id)) {
    return false;
  }

  position = skipSpaces(line, position);
  if (position == line.size()) {
    return false;
  }

  Person parsedPerson = { id, line.substr(position) };
  person = parsedPerson;
  return true;
}

tarasenko::ReadStats tarasenko::readPersons(std::istream& input, PersonStorage& storage)
{
  ReadStats stats = { 0, 0 };
  std::string line;
  while (std::getline(input, line)) {
    Person person = { 0, "" };
    if (!parsePersonLine(line, person) || containsId(storage, person.id)) {
      ++stats.ignored;
      continue;
    }

    appendPerson(storage, person);
    ++stats.accepted;
  }

  return stats;
}
