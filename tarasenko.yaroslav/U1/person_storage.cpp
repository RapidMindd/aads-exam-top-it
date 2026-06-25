#include "person_storage.hh"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace
{
  const std::size_t INITIAL_CAPACITY = 8;
  const std::size_t GROWTH_FACTOR = 2;

  void reserveStorage(tarasenko::PersonStorage& storage, std::size_t newCapacity)
  {
    if (newCapacity <= storage.capacity) {
      return;
    }

    std::unique_ptr< tarasenko::Person[] > newPersons(new tarasenko::Person[newCapacity]);
    for (std::size_t index = 0; index < storage.size; ++index) {
      newPersons[index] = storage.persons[index];
    }

    storage.persons.swap(newPersons);
    storage.capacity = newCapacity;
  }
}

tarasenko::PersonStorage tarasenko::makePersonStorage()
{
  PersonStorage storage = { nullptr, 0, 0 };
  return storage;
}

bool tarasenko::containsId(const PersonStorage& storage, std::size_t id)
{
  for (std::size_t index = 0; index < storage.size; ++index) {
    if (storage.persons[index].id == id) {
      return true;
    }
  }
  return false;
}

void tarasenko::appendPerson(PersonStorage& storage, const Person& person)
{
  if (storage.size == storage.capacity) {
    std::size_t newCapacity = INITIAL_CAPACITY;
    if (storage.capacity != 0) {
      if (storage.capacity > (std::numeric_limits< std::size_t >::max() / GROWTH_FACTOR)) {
        throw std::length_error("person storage capacity overflow");
      }
      newCapacity = storage.capacity * GROWTH_FACTOR;
    }
    reserveStorage(storage, newCapacity);
  }

  storage.persons[storage.size] = person;
  ++storage.size;
}
