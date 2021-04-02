#include <fstream>
#include <sstream>
#include <string>

inline std::string LoadString(const char* filename) {
  std::ifstream istream(filename, std::ios_base::in | std::ios_base::binary);
  std::stringstream sstream;
  sstream << istream.rdbuf();
  istream.close();
  return sstream.str();
}
