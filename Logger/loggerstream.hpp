#ifndef LOGGERSTREAM_HPP
#define LOGGERSTREAM_HPP

#include <sstream>
#include <string>

#include "logger.hpp"

class LoggerStream {
public:
  using LogFunc = bool (Logger::*)(const std::string&);

  explicit LoggerStream(LogFunc logFunc)
        : m_logFunc(logFunc) {}

  ~LoggerStream() {
    if (!m_buffer.str().empty() && Logger::isAlive()) {
      (Logger::instance().*m_logFunc)(m_buffer.str());
    }
  }

  template <typename T>
  LoggerStream& operator<<(const T& value) {
    m_buffer << value;
    return *this;
  }
private:
  std::ostringstream m_buffer;
  LogFunc m_logFunc;
};

#endif // LOGGERSTREAM_HPP
