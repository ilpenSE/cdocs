#include "logger.hpp"
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <iostream>

namespace fs = std::filesystem;

bool Logger::s_alive = false;
bool Logger::m_isLocalTime = false;

namespace coututil {
  void println(const std::string& msg) {
    std::cout << msg << std::endl;
  }

  void errorln(const std::string& msg) {
    std::cout << "ERROR: " << msg << std::endl;
  }
}

// initialization, opening file
bool Logger::initialize(const std::string& logsDir, bool isLocalTime) {
  // check if log dir exists and a directory
  bool dirExists = fs::exists(logsDir);
  if (dirExists && !fs::is_directory(logsDir)) {
    coututil::errorln("Given path is NOT a directory");
    return false;
  }

  if (!dirExists) {
    if (!fs::create_directories(logsDir)) {
      coututil::errorln("Logs folder cannot be created!");
      return false;
    }
  }
  m_isLocalTime = isLocalTime;

  // defining file name
  std::string fileName = getTime() + ".log";
  std::string filePath = logsDir + fileName;
  m_logFile.open(filePath.c_str(), std::ios::out | std::ios::app);

  // check defensively
  if (!m_logFile.is_open()) {
    coututil::errorln("Log file could not be opened");
    return false;
  }

  return true;
}

// main logging func
bool Logger::logPrivate(const std::string& msg, const std::string& level) {
  std::string message = getTime() + " [" + level + "] " + msg;
  coututil::println(message);
  m_logFile << message << '\n';

  if (!m_logFile.good()) {
    coututil::errorln("Logging failed!");
    return false;
  }
  return true;
}

// main log func's wrappers
bool Logger::log(const std::string& msg, const std::string& level) {
  return logPrivate(msg, level);
}

bool Logger::logInfo(const std::string& msg) {
  return logPrivate(msg, "INFO");
}

bool Logger::logError(const std::string& msg) {
  return logPrivate(msg, "ERROR");
}

bool Logger::logWarning(const std::string& msg) {
  return logPrivate(msg, "WARNING");
}

// timestamp wrapper - platform independent
std::string Logger::getTime() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};

#if defined(_WIN32)
    if (!m_isLocalTime) {
        gmtime_s(&tm, &t);
    } else {
        localtime_s(&tm, &t);
    }
#else
    if (!m_isLocalTime) {
        gmtime_r(&t, &tm);
    } else {
        localtime_r(&t, &tm);
    }
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y.%m.%d-%H:%M:%S");
    return oss.str();
}

// set alive false and close the file
Logger::~Logger() {
  s_alive = false;
  if (m_logFile.is_open() && m_logFile.good()) m_logFile.close();
}

Logger::Logger() { s_alive = true; }
