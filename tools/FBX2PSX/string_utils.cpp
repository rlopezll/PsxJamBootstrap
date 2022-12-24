#include "string_utils.h"
#include <sstream>
#include <algorithm>
#include <string> 
#include "file_utils.h"

namespace StringUtils {

  bool replaceStr(std::string& str, const std::string& from, const std::string& to) {
    if (from.length() == 0)
      return false;

    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
      return false;
    while (start_pos != std::string::npos) {
      str.replace(start_pos, from.length(), to);
      start_pos = str.find(from);
    }
    return true;
  }

  VStrings &split(const std::string &s, char delim, VStrings &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  }

  VFloats &splitToFloats(std::string s, char delim, VFloats &elems) {
    replaceStr(s, "\n", " ");
    replaceStr(s, "\r", " ");
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      if (item != "") {
        float value = std::stof(item);
        elems.push_back(value);
      }
    }
    return elems;
  }

  VInts &splitToInts(std::string s, char delim, VInts &elems) {
    replaceStr(s, "\n", " ");
    replaceStr(s, "\r", " ");
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      if (item != "") {
        int value = std::stoi(item);
        elems.push_back(value);
      }
    }
    return elems;
  }

  bool contains(const std::string &str, const std::string &str2) {
    std::size_t found = str.find(str2);
    return (found != std::string::npos);
  }

};
