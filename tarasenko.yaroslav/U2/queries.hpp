#ifndef TARASENKO_YAROSLAV_U2_QUERIES_HPP
#define TARASENKO_YAROSLAV_U2_QUERIES_HPP

#include <cstddef>
#include <iosfwd>

#include "domain.hpp"

namespace tarasenko
{
  void writeAnons(std::ostream& output, const Database& database);
  bool writeDescription(std::ostream& output, const Database& database, size_t id);
  bool writeMeetings(std::ostream& output, const Database& database, size_t id);
  bool writeLessMeetings(std::ostream& output,
      const Database& database,
      size_t time,
      size_t id);
  bool writeGreaterMeetings(std::ostream& output,
      const Database& database,
      size_t time,
      size_t id);
  bool writeCommonPersons(std::ostream& output,
      const Database& database,
      size_t first,
      size_t second);
  void writePersonsWithInfo(std::ostream& output, const Database& database);
}

#endif
