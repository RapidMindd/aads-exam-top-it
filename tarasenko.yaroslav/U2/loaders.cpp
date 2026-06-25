#include "loaders.hpp"

#include <string>

#include "parse_utils.hpp"

namespace tarasenko
{
  namespace
  {
  bool parsePersonLine(const std::string& line, size_t& id, std::string& info)
  {
    size_t position = 0;
    if (!tarasenko::readSize(line, position, id)) {
      return false;
    }

    position = tarasenko::skipSpaces(line, position);
    if (position == line.size()) {
      return false;
    }

    info = line.substr(position);
    return true;
  }

  bool parseMeetingLine(const std::string& line, tarasenko::Meeting& meeting)
  {
    size_t position = 0;
    if (!tarasenko::readSize(line, position, meeting.first)) {
      return false;
    }
    if (!tarasenko::readSize(line, position, meeting.second)) {
      return false;
    }
    if (!tarasenko::readSize(line, position, meeting.time)) {
      return false;
    }
    return tarasenko::isOnlySpaces(line, position);
  }
  }
}

void tarasenko::loadPersons(std::istream& input, Database& database)
{
  std::string line;
  while (std::getline(input, line)) {
    size_t id = 0;
    std::string info;
    if (!parsePersonLine(line, id, info) || hasPerson(database, id)) {
      continue;
    }
    setPersonInfo(database, id, info);
  }
}

bool tarasenko::loadMeetings(std::istream& input, Database& database)
{
  std::string line;
  while (std::getline(input, line)) {
    if (isBlankLine(line)) {
      continue;
    }

    Meeting meeting = { 0, 0, 0 };
    if (!parseMeetingLine(line, meeting)) {
      return false;
    }

    ensurePerson(database, meeting.first);
    ensurePerson(database, meeting.second);
    if (meeting.first != meeting.second) {
      appendMeeting(database, meeting);
    }
  }
  return true;
}
