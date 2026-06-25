#include "parse_utils.hpp"

#include <cctype>
#include <limits>

bool tarasenko::isSpace(char symbol)
{
  return std::isspace(static_cast< unsigned char >(symbol)) != 0;
}

std::size_t tarasenko::skipSpaces(const std::string& line, std::size_t position)
{
  while ((position < line.size()) && isSpace(line[position])) {
    ++position;
  }
  return position;
}

bool tarasenko::readSize(const std::string& line, std::size_t& position, std::size_t& value)
{
  position = skipSpaces(line, position);
  if ((position == line.size()) || !std::isdigit(static_cast< unsigned char >(line[position]))) {
    return false;
  }

  const std::size_t maxSize = std::numeric_limits< std::size_t >::max();
  std::size_t parsedValue = 0;
  while ((position < line.size()) && std::isdigit(static_cast< unsigned char >(line[position]))) {
    const std::size_t digit = static_cast< std::size_t >(line[position] - '0');
    if (parsedValue > ((maxSize - digit) / 10)) {
      return false;
    }
    parsedValue = (parsedValue * 10) + digit;
    ++position;
  }

  value = parsedValue;
  return true;
}

bool tarasenko::isOnlySpaces(const std::string& line, std::size_t position)
{
  position = skipSpaces(line, position);
  return position == line.size();
}

bool tarasenko::readQuotedText(const std::string& line, std::size_t& position, std::string& text)
{
  position = skipSpaces(line, position);
  if ((position == line.size()) || (line[position] != '"')) {
    return false;
  }

  const std::size_t first = position + 1;
  position = first;
  while ((position < line.size()) && (line[position] != '"')) {
    ++position;
  }
  if (position == line.size()) {
    return false;
  }

  text = line.substr(first, position - first);
  ++position;
  return true;
}

