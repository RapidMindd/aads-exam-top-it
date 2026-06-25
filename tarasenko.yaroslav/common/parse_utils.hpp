#ifndef TARASENKO_YAROSLAV_COMMON_PARSE_UTILS_HPP
#define TARASENKO_YAROSLAV_COMMON_PARSE_UTILS_HPP

#include <cstddef>
#include <string>

namespace tarasenko
{
  bool isSpace(char symbol);
  size_t skipSpaces(const std::string& line, size_t position);
  bool isBlankLine(const std::string& line);
  bool readSize(const std::string& line, size_t& position, size_t& value);
  bool isOnlySpaces(const std::string& line, size_t position);
  bool readQuotedText(const std::string& line, size_t& position, std::string& text);
}

#endif
