#ifndef ___BOOL_H__
#define ___BOOL_H__

#include <string>

namespace _Bool {
    inline std::string toString(bool b) {
        return b ? "true" : "false";
    }
    inline std::string toStringStatus(bool b) {
        return b ? "on" : "off";
    }
}
#endif