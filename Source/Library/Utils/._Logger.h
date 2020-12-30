#ifndef ___LOGGER_H__
#define ___LOGGER_H__
#include <iostream>
#include <iomanip> 
#include "./_String.h"
#include "./_DateTime.h"

namespace _Logger {
    enum Level {
        INFO, DEBUG, WARNING, ERROR
    };
    std::string stringLevel[] {
        "INFO", "DEBUG", "WARNING", "ERROR"
    };
    struct Logger {
        public:
            Logger& operator<<(const std::string& msg)
            {
                std::cout << msg; // also print the level etc.
                return *this;
            }

            static Logger& log(Level n)
            {
                std::cout << _DateTime::grabDate();
                std::cout << " | ";
                std::cout << _DateTime::grabTime();
                std::cout << " | ";
                std::cout << std::setw(8) << std::left << stringLevel[n] << " ";
                std::cout << " | ";
                static Logger lm;
                return lm;
            }
    };
    Logger& LOG(Level n) {
        return Logger::log(n);
    }
}
#endif