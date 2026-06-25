#ifndef TARASENKO_YAROSLAV_U1_PERSON_STORAGE_HH
#define TARASENKO_YAROSLAV_U1_PERSON_STORAGE_HH

#include <cstddef>
#include <memory>

#include "person.hh"

namespace tarasenko
{
  struct PersonStorage
  {
    std::unique_ptr< Person[] > persons;
    std::size_t size;
    std::size_t capacity;
  };

  PersonStorage makePersonStorage();
  bool containsId(const PersonStorage& storage, std::size_t id);
  void appendPerson(PersonStorage& storage, const Person& person);
}

#endif
