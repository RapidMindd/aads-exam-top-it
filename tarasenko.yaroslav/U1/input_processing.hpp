#ifndef TARASENKO_YAROSLAV_U1_INPUT_PROCESSING_HPP
#define TARASENKO_YAROSLAV_U1_INPUT_PROCESSING_HPP

#include <cstddef>
#include <iosfwd>
#include <string>

#include "person.hpp"
#include "person_storage.hpp"

namespace tarasenko
{
  struct ReadStats
  {
    size_t accepted;
    size_t ignored;
  };

  bool parsePersonLine(const std::string& line, Person& person);
  ReadStats readPersons(std::istream& input, PersonStorage& storage);
}

#endif
