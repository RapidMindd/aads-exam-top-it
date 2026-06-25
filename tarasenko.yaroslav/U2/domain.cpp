#include "domain.hpp"

tarasenko::Database tarasenko::makeDatabase()
{
  Database database = { makeDynamicArray< PersonRecord >(), makeDynamicArray< Meeting >() };
  return database;
}

void tarasenko::destroyDatabase(Database& database)
{
  destroyDynamicArray(database.persons);
  destroyDynamicArray(database.meetings);
}

int tarasenko::findPersonIndex(const Database& database, std::size_t id)
{
  for (std::size_t index = 0; index < database.persons.size; ++index) {
    if (database.persons.data[index].id == id) {
      return static_cast< int >(index);
    }
  }
  return -1;
}

bool tarasenko::hasPerson(const Database& database, std::size_t id)
{
  return findPersonIndex(database, id) >= 0;
}

bool tarasenko::hasPersonInfo(const Database& database, std::size_t id)
{
  const int index = findPersonIndex(database, id);
  return (index >= 0) && database.persons.data[index].hasInfo;
}

void tarasenko::ensurePerson(Database& database, std::size_t id)
{
  if (hasPerson(database, id)) {
    return;
  }

  PersonRecord person = { id, "", false };
  appendDynamicArray(database.persons, person);
}

void tarasenko::setPersonInfo(Database& database, std::size_t id, const std::string& info)
{
  const int index = findPersonIndex(database, id);
  if (index < 0) {
    ensurePerson(database, id);
    setPersonInfo(database, id, info);
    return;
  }

  database.persons.data[index].info = info;
  database.persons.data[index].hasInfo = true;
}

void tarasenko::appendMeeting(Database& database, const Meeting& meeting)
{
  appendDynamicArray(database.meetings, meeting);
}

bool tarasenko::mergeAnonPerson(Database& database, std::size_t anonId, std::size_t id)
{
  const int anonIndex = findPersonIndex(database, anonId);
  const int personIndex = findPersonIndex(database, id);
  if ((anonIndex < 0) || (personIndex < 0)) {
    return false;
  }
  if (database.persons.data[anonIndex].hasInfo || !database.persons.data[personIndex].hasInfo) {
    return false;
  }

  std::size_t writeIndex = 0;
  for (std::size_t index = 0; index < database.meetings.size; ++index) {
    Meeting meeting = database.meetings.data[index];
    if (meeting.first == anonId) {
      meeting.first = id;
    }
    if (meeting.second == anonId) {
      meeting.second = id;
    }
    if (meeting.first != meeting.second) {
      database.meetings.data[writeIndex] = meeting;
      ++writeIndex;
    }
  }
  database.meetings.size = writeIndex;

  for (std::size_t index = static_cast< std::size_t >(anonIndex) + 1;
      index < database.persons.size;
      ++index) {
    database.persons.data[index - 1] = database.persons.data[index];
  }
  --database.persons.size;
  return true;
}
