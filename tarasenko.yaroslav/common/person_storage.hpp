#ifndef TARASENKO_YAROSLAV_COMMON_PERSON_STORAGE_HPP
#define TARASENKO_YAROSLAV_COMMON_PERSON_STORAGE_HPP

#include <cstddef>

#include "dynamic_array.hpp"
#include "person.hpp"

namespace tarasenko
{
  using PersonStorage = DynamicArray< Person >;

  PersonStorage makePersonStorage();
  void destroyPersonStorage(PersonStorage& storage);
  bool containsId(const PersonStorage& storage, size_t id);
  void appendPerson(PersonStorage& storage, const Person& person);
}

#endif
