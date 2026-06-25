#include "person_output.hpp"

#include <ostream>

void tarasenko::writePersons(std::ostream& output, const PersonStorage& storage)
{
  for (size_t index = 0; index < storage.size; ++index) {
    output << storage.data[index].id << ' ' << storage.data[index].info << '\n';
  }
}
