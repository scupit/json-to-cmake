#include "helpers/StringHelper.hpp"

std::vector<std::string> StringHelper::split(const std::string& toSplit, const std::string& splitter) {
  std::vector<std::string> sections;

  size_t 
    currentPos = 0,
    nextPos = toSplit.find(splitter);

  while (nextPos != std::string::npos) {
    if (nextPos - currentPos > 1) {
      sections.push_back(toSplit.substr(currentPos, nextPos - currentPos));
    }

    currentPos = nextPos + splitter.size();
    nextPos = toSplit.find(splitter, currentPos);
  }

  if (currentPos < toSplit.size() - 1) {
    sections.push_back(toSplit.substr(currentPos));
  }
  return sections;
}

std::string StringHelper::join(const std::vector<std::string>& sections, const std::string& sep) {
  std::string joinedString;

  auto iter = sections.begin();
  joinedString.append(*iter);
  ++iter;

  for (; iter != sections.end(); ++iter) {
    joinedString.append(sep);
    joinedString.append(*iter);
  }
  return joinedString;
}

std::string StringHelper::replaceAll(std::string str, const std::string& toReplace, const std::string& replacer) {
  if (toReplace == replacer)
    return str;

  size_t index = 0;

  while ((index = str.find(toReplace, index)) != std::string::npos) {
    str.replace(index, toReplace.size(), replacer);
  }
  return str;
}