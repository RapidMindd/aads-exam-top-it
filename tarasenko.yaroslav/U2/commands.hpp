#ifndef TARASENKO_YAROSLAV_U2_COMMANDS_HPP
#define TARASENKO_YAROSLAV_U2_COMMANDS_HPP

#include <iosfwd>

#include "domain.hpp"

namespace tarasenko
{
  void processCommands(std::istream& input, std::ostream& output, Database& database);
}

#endif
