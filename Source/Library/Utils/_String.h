#ifndef ___STRING_H__
#define ___STRING_H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Engine {
    struct stringbuilder {
        std::stringstream ss;
        template <typename T> stringbuilder &operator<<(const T &data) {
            ss << data;
            return *this;
        }
        operator std::string() { return ss.str(); }
    };

    inline std::string stringLower(std::string tempString) {
        for (unsigned int i = 0; i < tempString.length(); i++) {
            tempString[i] = std::tolower(tempString[i]);
        }
        return tempString;
    }

    inline bool toBool(std::string tempString) { return stringLower(tempString) == "true"; }

    inline std::string stringReplace(std::string s0, std::string s1, std::string s2) {
        while (s0.find(s1) != std::string::npos) {
            int s1Loc = s0.find(s1);
            s0.replace(s1Loc, s1.size(), s2);
        }
        return s0;
    }

    inline std::vector<std::string> stringSplit(std::string s0, std::string delim) {
        std::vector<std::string> resp;
        while (s0.find(delim) != std::string::npos) {
            resp.push_back(s0.substr(0, s0.find(delim)));
            s0 = s0.substr(s0.find(delim) + 1, s0.size() - (s0.find(delim)));
        }
        resp.push_back(s0);
        return resp;
    }

    inline std::string toRawString(std::string const &in) {
        std::string ret = in;
        while (ret.find('\n') != std::string::npos) {
            auto p = ret.find('\n');
            if (p != ret.npos) {
                ret.replace(p, 1, "\\n");
            }
        }
        return ret;
    }

    inline std::string fromRawString(std::string const &in) {
        std::string ret = in;
        while (ret.find('\n') != std::string::npos) {
            auto p = ret.find("\\n");
            char t = '\n';
            std::string s = &t;
            if (p != ret.npos) {
                ret.replace(p, 2, s);
            }
        }
        return ret;
    }
} // namespace Engine
#endif