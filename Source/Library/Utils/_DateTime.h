#ifndef ___DATETIME_H__
#define ___DATETIME_H__

#include <ctime>
#include <stdio.h>
#include <string>

namespace _DateTime {
    inline std::string grabDate() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[11];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer, 11, "%d-%m-%Y", timeinfo);
        std::string date(buffer);
        return date;
    }

    inline std::string grabTime() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[11];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer, 11, "%H:%M:%S", timeinfo);
        std::string time(buffer);
        return time;
    }
}
#endif