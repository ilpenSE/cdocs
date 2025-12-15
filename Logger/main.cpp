#include <iostream>
#include "macros.hpp"
#include "logger.hpp"
#include <string>

/**
This is NOT a JSON logger, its human-readable logger
Usage is provided below.

the path variable should be ended with slash (/), otherwise it can cause errors
if isLocalTime set to true, it fetches your time but if you set it false it fetches UTC datetime
*/
int main(int argc, char** argv) {
  std::string path = "logs/";
  bool isLocalTime = true;
  if (argc >= 3) {
    path = argv[1];
    isLocalTime = std::string(argv[2]) == "true";
  }
  if (!Logger::instance().initialize(path, isLocalTime)) {
    std::cout << "[MAIN] Logger init failed" << std::endl;
    return -1;
  }

  linfo << "Hello World!";
  lerror << "Some error occured";
  lwarning << "Some warning";
  return 0;
}
