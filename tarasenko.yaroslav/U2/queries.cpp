#include "queries.hpp"

#include <memory>
#include <ostream>

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
    for (std::size_t index = 0; index < database.meetingCount; ++index) {
      if ((database.meetings[index].first == id) || (database.meetings[index].second == id)) {
        ++count;
      }
    }
    return count;
  }

  std::size_t fillMeetingViews(const tarasenko::Database& database,
      std::size_t id,
      MeetingView meetings[],
      bool (*isAccepted)(std::size_t, std::size_t),
      std::size_t limitTime)
  {
    std::size_t writeIndex = 0;
    for (std::size_t index = 0; index < database.meetingCount; ++index) {
      const tarasenko::Meeting meeting = database.meetings[index];
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
        meetings[writeIndex] = view;
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
    for (std::size_t index = 0; index < database.meetingCount; ++index) {
      const tarasenko::Meeting meeting = database.meetings[index];
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
    std::unique_ptr< MeetingView[] > meetings(new MeetingView[maxCount]);
    const std::size_t size = fillMeetingViews(database, id, meetings.get(), isAccepted, limitTime);

    sortMeetingViews(meetings.get(), size);
    for (std::size_t index = 0; index < size; ++index) {
      output << meetings[index].person << ' ' << meetings[index].time << '\n';
    }
    return true;
  }
  }
}

void tarasenko::writeAnons(std::ostream& output, const Database& database)
{
  std::unique_ptr< PersonRecord[] > persons(new PersonRecord[database.personCount]);
  std::size_t size = 0;
  for (std::size_t index = 0; index < database.personCount; ++index) {
    if (!database.persons[index].hasInfo) {
      persons[size] = database.persons[index];
      ++size;
    }
  }

  sortPersons(persons.get(), size);
  for (std::size_t index = 0; index < size; ++index) {
    output << persons[index].id << '\n';
  }
}

bool tarasenko::writeDescription(std::ostream& output, const Database& database, std::size_t id)
{
  const int index = findPersonIndex(database, id);
  if (index < 0) {
    return false;
  }
  if (database.persons[index].hasInfo) {
    output << database.persons[index].info << '\n';
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

  std::unique_ptr< std::size_t[] > persons(new std::size_t[database.personCount]);
  std::size_t size = 0;
  for (std::size_t index = 0; index < database.personCount; ++index) {
    const std::size_t id = database.persons[index].id;
    if ((id != first) && (id != second) && hasMeetingWith(database, first, id)
        && hasMeetingWith(database, second, id) && !containsSize(persons.get(), size, id)) {
      persons[size] = id;
      ++size;
    }
  }

  sortSizes(persons.get(), size);
  for (std::size_t index = 0; index < size; ++index) {
    output << persons[index] << '\n';
  }
  return true;
}

void tarasenko::writePersonsWithInfo(std::ostream& output, const Database& database)
{
  for (std::size_t index = 0; index < database.personCount; ++index) {
    if (database.persons[index].hasInfo) {
      output << database.persons[index].id << ' ' << database.persons[index].info << '\n';
    }
  }
}
