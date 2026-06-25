#ifndef TARASENKO_YAROSLAV_COMMON_PERSON_OUTPUT_HPP
#define TARASENKO_YAROSLAV_COMMON_PERSON_OUTPUT_HPP

#include <iosfwd>

#include "person_storage.hpp"

namespace tarasenko
{
  void writePersons(std::ostream& output, const PersonStorage& storage);
}

#endif
