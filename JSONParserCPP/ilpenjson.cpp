#include "ilpenjson.hpp"
#include <fstream>
#include <string>
#include <iostream>

bool JsonReader::read_file(std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cout << "Failed to open JSON file: " << file_path << std::endl;
    return false;
  }

  std::string content(
    (std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>()
  );

  return true;
}
