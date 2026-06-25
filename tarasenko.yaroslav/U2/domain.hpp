#ifndef TARASENKO_YAROSLAV_U2_DOMAIN_HPP
#define TARASENKO_YAROSLAV_U2_DOMAIN_HPP

#include <cstddef>
#include <string>

#include "dynamic_array.hpp"

namespace tarasenko
{
  struct PersonRecord
  {
    std::size_t id;
    std::string info;
    bool hasInfo;
  };

  struct Meeting
  {
    std::size_t first;
    std::size_t second;
    std::size_t time;
  };

  struct Database
  {
    DynamicArray< PersonRecord > persons;
    DynamicArray< Meeting > meetings;
  };

  Database makeDatabase();
  void destroyDatabase(Database& database);
  int findPersonIndex(const Database& database, std::size_t id);
  bool hasPerson(const Database& database, std::size_t id);
  bool hasPersonInfo(const Database& database, std::size_t id);
  void ensurePerson(Database& database, std::size_t id);
  void setPersonInfo(Database& database, std::size_t id, const std::string& info);
  void appendMeeting(Database& database, const Meeting& meeting);
  bool mergeAnonPerson(Database& database, std::size_t anonId, std::size_t id);
}

#endif
