#ifndef TARASENKO_YAROSLAV_U1_INPUT_PROCESSING_HH
#define TARASENKO_YAROSLAV_U1_INPUT_PROCESSING_HH

#include <cstddef>
#include <iosfwd>
#include <string>

#include "person.hh"
#include "person_storage.hh"

namespace tarasenko
{
  struct ReadStats
  {
    std::size_t accepted;
    std::size_t ignored;
  };

  bool parsePersonLine(const std::string& line, Person& person);
  ReadStats readPersons(std::istream& input, PersonStorage& storage);
}

#endif
