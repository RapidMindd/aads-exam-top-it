#include "input_processing.hpp"
#include <istream>
#include "parse_utils.hpp"

bool tarasenko::parsePersonLine(const std::string& line, Person& person)
{
  size_t position = 0;
  size_t id = 0;
  if (!readSize(line, position, id)) {
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
    if (isBlankLine(line)) {
      continue;
    }

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
