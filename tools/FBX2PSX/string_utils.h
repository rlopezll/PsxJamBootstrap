#ifndef INC_STRING_UTILS
#define INC_STRING_UTILS

#include <vector>
#include <string>
#include <sstream>

typedef std::vector<std::string> VStrings;
typedef std::vector<float> VFloats;
typedef std::vector<int> VInts;

namespace StringUtils {
  VStrings &split(const std::string &s, char delim, VStrings &elems);
  VFloats &splitToFloats(std::string s, char delim, VFloats &elems);
  VInts &splitToInts(std::string s, char delim, VInts &elems);
  bool contains(const std::string &s, const std::string &f);
  bool replaceStr(std::string& str, const std::string& from, const std::string& to);
  template<class T>
  std::string toString(T i)
  {
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();
    return s;
  }
};

#endif
