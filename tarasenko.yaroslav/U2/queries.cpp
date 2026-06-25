#include "queries.hpp"

#include <ostream>

#include "dynamic_array.hpp"

namespace tarasenko
{
  namespace
  {
  struct MeetingView
  {
    std::size_t person;
    std::size_t time;
  };

  bool isLessPerson(const tarasenko::PersonRecord& left, const tarasenko::PersonRecord& right)
  {
    return left.id < right.id;
  }

  bool isLessMeetingView(const MeetingView& left, const MeetingView& right)
  {
    if (left.person != right.person) {
      return left.person < right.person;
    }
    return left.time < right.time;
  }

  void sortPersons(tarasenko::PersonRecord persons[], std::size_t size)
  {
    for (std::size_t index = 1; index < size; ++index) {
      const tarasenko::PersonRecord person = persons[index];
      std::size_t position = index;
      while ((position > 0) && isLessPerson(person, persons[position - 1])) {
        persons[position] = persons[position - 1];
        --position;
      }
      persons[position] = person;
    }
  }

  void sortMeetingViews(MeetingView meetings[], std::size_t size)
  {
    for (std::size_t index = 1; index < size; ++index) {
      const MeetingView meeting = meetings[index];
      std::size_t position = index;
      while ((position > 0) && isLessMeetingView(meeting, meetings[position - 1])) {
        meetings[position] = meetings[position - 1];
        --position;
      }
      meetings[position] = meeting;
    }
  }

  std::size_t countMeetings(const tarasenko::Database& database, std::size_t id)
  {
    std::size_t count = 0;
    for (std::size_t index = 0; index < database.meetings.size; ++index) {
      if ((database.meetings.data[index].first == id)
          || (database.meetings.data[index].second == id)) {
        ++count;
      }
    }
    return count;
  }

  std::size_t fillMeetingViews(const tarasenko::Database& database,
      std::size_t id,
      tarasenko::DynamicArray< MeetingView >& meetings,
      bool (*isAccepted)(std::size_t, std::size_t),
      std::size_t limitTime)
  {
    std::size_t writeIndex = 0;
    for (std::size_t index = 0; index < database.meetings.size; ++index) {
      const tarasenko::Meeting meeting = database.meetings.data[index];
      std::size_t other = 0;
      if (meeting.first == id) {
        other = meeting.second;
      } else if (meeting.second == id) {
        other = meeting.first;
      } else {
        continue;
      }
      if (isAccepted(meeting.time, limitTime)) {
        const MeetingView view = { other, meeting.time };
        tarasenko::appendDynamicArray(meetings, view);
        ++writeIndex;
      }
    }
    return writeIndex;
  }

  bool acceptAny(std::size_t, std::size_t)
  {
    return true;
  }

  bool acceptLess(std::size_t time, std::size_t limit)
  {
    return time < limit;
  }

  bool acceptGreater(std::size_t time, std::size_t limit)
  {
    return time > limit;
  }

  bool hasMeetingWith(const tarasenko::Database& database, std::size_t id, std::size_t other)
  {
    for (std::size_t index = 0; index < database.meetings.size; ++index) {
      const tarasenko::Meeting meeting = database.meetings.data[index];
      if (((meeting.first == id) && (meeting.second == other))
          || ((meeting.first == other) && (meeting.second == id))) {
        return true;
      }
    }
    return false;
  }

  bool containsSize(const std::size_t values[], std::size_t size, std::size_t value)
  {
    for (std::size_t index = 0; index < size; ++index) {
      if (values[index] == value) {
        return true;
      }
    }
    return false;
  }

  void sortSizes(std::size_t values[], std::size_t size)
  {
    for (std::size_t index = 1; index < size; ++index) {
      const std::size_t value = values[index];
      std::size_t position = index;
      while ((position > 0) && (value < values[position - 1])) {
        values[position] = values[position - 1];
        --position;
      }
      values[position] = value;
    }
  }

  bool writeFilteredMeetings(std::ostream& output,
      const tarasenko::Database& database,
      std::size_t id,
      bool (*isAccepted)(std::size_t, std::size_t),
      std::size_t limitTime)
  {
    if (!tarasenko::hasPerson(database, id)) {
      return false;
    }

    const std::size_t maxCount = countMeetings(database, id);
    DynamicArray< MeetingView > meetings = makeDynamicArray< MeetingView >();
    reserveDynamicArray(meetings, maxCount);
    const std::size_t size = fillMeetingViews(database, id, meetings, isAccepted, limitTime);

    sortMeetingViews(meetings.data, size);
    if (size == 0) {
      output << '\n';
    } else {
      for (std::size_t index = 0; index < size; ++index) {
        output << meetings.data[index].person << ' ' << meetings.data[index].time << '\n';
      }
    }
    destroyDynamicArray(meetings);
    return true;
  }
  }
}

void tarasenko::writeAnons(std::ostream& output, const Database& database)
{
  DynamicArray< PersonRecord > persons = makeDynamicArray< PersonRecord >();
  reserveDynamicArray(persons, database.persons.size);
  for (std::size_t index = 0; index < database.persons.size; ++index) {
    if (!database.persons.data[index].hasInfo) {
      appendDynamicArray(persons, database.persons.data[index]);
    }
  }

  sortPersons(persons.data, persons.size);
  if (persons.size == 0) {
    output << '\n';
  } else {
    for (std::size_t index = 0; index < persons.size; ++index) {
      output << persons.data[index].id << '\n';
    }
  }
  destroyDynamicArray(persons);
}

bool tarasenko::writeDescription(std::ostream& output, const Database& database, std::size_t id)
{
  const int index = findPersonIndex(database, id);
  if (index < 0) {
    return false;
  }
  if (database.persons.data[index].hasInfo) {
    output << database.persons.data[index].info << '\n';
  } else {
    output << "<ANON>\n";
  }
  return true;
}

bool tarasenko::writeMeetings(std::ostream& output, const Database& database, std::size_t id)
{
  return writeFilteredMeetings(output, database, id, acceptAny, 0);
}

bool tarasenko::writeLessMeetings(std::ostream& output,
    const Database& database,
    std::size_t time,
    std::size_t id)
{
  return writeFilteredMeetings(output, database, id, acceptLess, time);
}

bool tarasenko::writeGreaterMeetings(std::ostream& output,
    const Database& database,
    std::size_t time,
    std::size_t id)
{
  return writeFilteredMeetings(output, database, id, acceptGreater, time);
}

bool tarasenko::writeCommonPersons(std::ostream& output,
    const Database& database,
    std::size_t first,
    std::size_t second)
{
  if (!hasPerson(database, first) || !hasPerson(database, second)) {
    return false;
  }

  DynamicArray< std::size_t > persons = makeDynamicArray< std::size_t >();
  reserveDynamicArray(persons, database.persons.size);
  for (std::size_t index = 0; index < database.persons.size; ++index) {
    const std::size_t id = database.persons.data[index].id;
    if ((id != first) && (id != second) && hasMeetingWith(database, first, id)
        && hasMeetingWith(database, second, id)
        && !containsSize(persons.data, persons.size, id)) {
      appendDynamicArray(persons, id);
    }
  }

  sortSizes(persons.data, persons.size);
  if (persons.size == 0) {
    output << '\n';
  } else {
    for (std::size_t index = 0; index < persons.size; ++index) {
      output << persons.data[index] << '\n';
    }
  }
  destroyDynamicArray(persons);
  return true;
}

void tarasenko::writePersonsWithInfo(std::ostream& output, const Database& database)
{
  for (std::size_t index = 0; index < database.persons.size; ++index) {
    if (database.persons.data[index].hasInfo) {
      output << database.persons.data[index].id << ' ' << database.persons.data[index].info << '\n';
    }
  }
}
