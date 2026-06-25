#include "domain.hpp"

#include <limits>
#include <stdexcept>

namespace tarasenko
{
  namespace
  {
  const std::size_t INITIAL_CAPACITY = 8;
  const std::size_t GROWTH_FACTOR = 2;

  std::size_t growCapacity(std::size_t capacity)
  {
    if (capacity == 0) {
      return INITIAL_CAPACITY;
    }
    if (capacity > (std::numeric_limits< std::size_t >::max() / GROWTH_FACTOR)) {
      throw std::length_error("capacity overflow");
    }
    return capacity * GROWTH_FACTOR;
  }

  void reservePersons(tarasenko::Database& database, std::size_t capacity)
  {
    if (capacity <= database.personCapacity) {
      return;
    }

    std::unique_ptr< tarasenko::PersonRecord[] > persons(new tarasenko::PersonRecord[capacity]);
    for (std::size_t index = 0; index < database.personCount; ++index) {
      persons[index] = database.persons[index];
    }
    database.persons.swap(persons);
    database.personCapacity = capacity;
  }

  void reserveMeetings(tarasenko::Database& database, std::size_t capacity)
  {
    if (capacity <= database.meetingCapacity) {
      return;
    }

    std::unique_ptr< tarasenko::Meeting[] > meetings(new tarasenko::Meeting[capacity]);
    for (std::size_t index = 0; index < database.meetingCount; ++index) {
      meetings[index] = database.meetings[index];
    }
    database.meetings.swap(meetings);
    database.meetingCapacity = capacity;
  }
  }
}

tarasenko::Database tarasenko::makeDatabase()
{
  Database database = { nullptr, 0, 0, nullptr, 0, 0 };
  return database;
}

int tarasenko::findPersonIndex(const Database& database, std::size_t id)
{
  for (std::size_t index = 0; index < database.personCount; ++index) {
    if (database.persons[index].id == id) {
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
  return (index >= 0) && database.persons[index].hasInfo;
}

void tarasenko::ensurePerson(Database& database, std::size_t id)
{
  if (hasPerson(database, id)) {
    return;
  }

  if (database.personCount == database.personCapacity) {
    reservePersons(database, growCapacity(database.personCapacity));
  }

  PersonRecord person = { id, "", false };
  database.persons[database.personCount] = person;
  ++database.personCount;
}

void tarasenko::setPersonInfo(Database& database, std::size_t id, const std::string& info)
{
  const int index = findPersonIndex(database, id);
  if (index < 0) {
    ensurePerson(database, id);
    setPersonInfo(database, id, info);
    return;
  }

  database.persons[index].info = info;
  database.persons[index].hasInfo = true;
}

void tarasenko::appendMeeting(Database& database, const Meeting& meeting)
{
  if (database.meetingCount == database.meetingCapacity) {
    reserveMeetings(database, growCapacity(database.meetingCapacity));
  }

  database.meetings[database.meetingCount] = meeting;
  ++database.meetingCount;
}

bool tarasenko::mergeAnonPerson(Database& database, std::size_t anonId, std::size_t id)
{
  const int anonIndex = findPersonIndex(database, anonId);
  const int personIndex = findPersonIndex(database, id);
  if ((anonIndex < 0) || (personIndex < 0)) {
    return false;
  }
  if (database.persons[anonIndex].hasInfo || !database.persons[personIndex].hasInfo) {
    return false;
  }

  std::size_t writeIndex = 0;
  for (std::size_t index = 0; index < database.meetingCount; ++index) {
    Meeting meeting = database.meetings[index];
    if (meeting.first == anonId) {
      meeting.first = id;
    }
    if (meeting.second == anonId) {
      meeting.second = id;
    }
    if (meeting.first != meeting.second) {
      database.meetings[writeIndex] = meeting;
      ++writeIndex;
    }
  }
  database.meetingCount = writeIndex;

  for (std::size_t index = static_cast< std::size_t >(anonIndex) + 1;
      index < database.personCount;
      ++index) {
    database.persons[index - 1] = database.persons[index];
  }
  --database.personCount;
  return true;
}
