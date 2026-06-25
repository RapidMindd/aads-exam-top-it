#ifndef TARASENKO_YAROSLAV_U2_LOADERS_HPP
#define TARASENKO_YAROSLAV_U2_LOADERS_HPP

#include <istream>

#include "domain.hpp"

namespace tarasenko
{
  void loadPersons(std::istream& input, Database& database);
  bool loadMeetings(std::istream& input, Database& database);
}

#endif
