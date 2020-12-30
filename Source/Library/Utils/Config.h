#ifndef __CONFIG_h__
#define __CONFIG_h__

#include "../../Third-Party/json.hpp"
#include "_String.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string.h>
#include <unordered_map>
#include <vector>
typedef nlohmann::json json;

class Config {
  public:
    Config() {}
    json settings;
    int load(const char *configLocation)
    {
        try {
            std::ifstream ifs(configLocation);
            settings = json::parse(ifs);
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
};
#endif