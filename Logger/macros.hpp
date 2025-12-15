#ifndef MACROS_HPP
#define MACROS_HPP

#include "logger.hpp"
#include "loggerstream.hpp"

#define linfo LoggerStream(&Logger::logInfo)
#define lerror LoggerStream(&Logger::logError)
#define lwarning LoggerStream(&Logger::logWarning)

#endif // MACROS_HPP
