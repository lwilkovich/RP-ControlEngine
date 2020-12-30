#ifndef __CONFIG_h__
#define __CONFIG_h__

#include <string.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <unordered_map>
#include <vector>
#include "../../Third-Party/json.hpp"
typedef nlohmann::json json;

class Config {
   public:
    Config() {}
    json settings;
    int load(const char *configLocation) {
        try {
            std::ifstream ifs(configLocation);
            settings = json::parse(ifs);
        }
        catch (...) {
            return 1;
        }
        return 0;
    } 
};
#endif