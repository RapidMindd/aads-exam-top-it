#include "person_storage.hpp"

tarasenko::PersonStorage tarasenko::makePersonStorage()
{
  return makeDynamicArray< Person >();
}

void tarasenko::destroyPersonStorage(PersonStorage& storage)
{
  destroyDynamicArray(storage);
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
  appendDynamicArray(storage, person);
}
