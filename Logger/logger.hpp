#ifndef LOGGER_HPP
#define LOGGER_HPP

/*
  PURE C++ LOGGER MEYERS SINGLETON

  Macros.hpp has pre-defined macros without any paranthesis

  Usage:
    #include "macros.hpp" // -> FOR MACROS
    #include "logger.hpp" // -> FOR INIT
    int main() {
      std::string logs_dir = "logs/"; // -> IT HAS TO BE DIRECTORY
      
      Logger::instance().initialize(logs_dir, true); // -> initializes with logs and isLocalTime, REQUIRED
                                               // you can check its return value:
                                               // bool isLoggerInitialized = Logger::instance().initialize(logs_dir);
                                               // if (!isLoggerInitialized) {
                                               //   throw std::runtime_error("Failed to initialize logger");
                                               // }
      linfo << "Hello World!";
    }
*/

#include <fstream>
#include <string>

namespace coututil {
  void println(const std::string& msg);
  void errorln(const std::string& msg);
}

class Logger {
  public:
    // meyers singleton
    static Logger& instance() {
      static Logger _ins;
      return _ins;
    }
    static bool isAlive() { return s_alive; }

    bool initialize(const std::string& logsDir = "logs/", bool isLocalTime = false);

    // log funcs
    bool logInfo(const std::string& msg);
    bool logError(const std::string& msg);
    bool logWarning(const std::string& msg);
    bool log(const std::string& msg, const std::string& level);
  private:
    bool logPrivate(const std::string& msg, const std::string& level);
    std::string getTime();

    // logger vars
    static bool s_alive;
    std::ofstream m_logFile;
    static bool m_isLocalTime;

    // singleton configs
    explicit Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_HPP
