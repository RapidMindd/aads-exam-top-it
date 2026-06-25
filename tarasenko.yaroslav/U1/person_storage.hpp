#ifndef TARASENKO_YAROSLAV_U1_PERSON_STORAGE_HPP
#define TARASENKO_YAROSLAV_U1_PERSON_STORAGE_HPP

#include <cstddef>

#include "person.hpp"

namespace tarasenko
{
  struct PersonStorage
  {
    Person* data;
    std::size_t size;
    std::size_t capacity;
  };

  PersonStorage makePersonStorage();
  void destroyPersonStorage(PersonStorage& storage);
  bool containsId(const PersonStorage& storage, std::size_t id);
  void appendPerson(PersonStorage& storage, const Person& person);
}

#endif
