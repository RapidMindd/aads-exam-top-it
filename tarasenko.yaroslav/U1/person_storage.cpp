#include "person_storage.hpp"

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

    tarasenko::Person* newData = new tarasenko::Person[newCapacity];
    try {
      for (std::size_t index = 0; index < storage.size; ++index) {
        newData[index] = storage.data[index];
      }
    } catch (...) {
      delete[] newData;
      throw;
    }

    delete[] storage.data;
    storage.data = newData;
    storage.capacity = newCapacity;
  }
}

tarasenko::PersonStorage tarasenko::makePersonStorage()
{
  PersonStorage storage = { nullptr, 0, 0 };
  return storage;
}

void tarasenko::destroyPersonStorage(PersonStorage& storage)
{
  delete[] storage.data;
  storage.data = nullptr;
  storage.size = 0;
  storage.capacity = 0;
}

bool tarasenko::containsId(const PersonStorage& storage, std::size_t id)
{
  for (std::size_t index = 0; index < storage.size; ++index) {
    if (storage.data[index].id == id) {
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

  storage.data[storage.size] = person;
  ++storage.size;
}
