#ifndef TARASENKO_YAROSLAV_U2_PARSE_UTILS_HPP
#define TARASENKO_YAROSLAV_U2_PARSE_UTILS_HPP

#include <cstddef>
#include <string>

namespace tarasenko
{
  bool isSpace(char symbol);
  std::size_t skipSpaces(const std::string& line, std::size_t position);
  bool readSize(const std::string& line, std::size_t& position, std::size_t& value);
  bool isOnlySpaces(const std::string& line, std::size_t position);
  bool readQuotedText(const std::string& line, std::size_t& position, std::string& text);
}

#endif
