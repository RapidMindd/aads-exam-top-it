#ifndef TARASENKO_YAROSLAV_U2_DOMAIN_HPP
#define TARASENKO_YAROSLAV_U2_DOMAIN_HPP

#include <cstddef>
#include <string>

#include "dynamic_array.hpp"

namespace tarasenko
{
  struct PersonRecord
  {
    size_t id;
    std::string info;
    bool hasInfo;
  };

  struct Meeting
  {
    size_t first;
    size_t second;
    size_t time;
  };

  struct Database
  {
    DynamicArray< PersonRecord > persons;
    DynamicArray< Meeting > meetings;
  };

  Database makeDatabase();
  void destroyDatabase(Database& database);
  int findPersonIndex(const Database& database, size_t id);
  bool hasPerson(const Database& database, size_t id);
  bool hasPersonInfo(const Database& database, size_t id);
  void ensurePerson(Database& database, size_t id);
  void setPersonInfo(Database& database, size_t id, const std::string& info);
  void appendMeeting(Database& database, const Meeting& meeting);
  bool mergeAnonPerson(Database& database, size_t anonId, size_t id);
}

#endif
