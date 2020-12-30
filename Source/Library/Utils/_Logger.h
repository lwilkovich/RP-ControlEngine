#ifndef ___LOGGER_H__
#define ___LOGGER_H__

#include "../Static/LogQueue.h"

#define _SYS "System"
#define _INFO "Info"
#define _DEBUG "Debug"
#define _WARNING "Warning"
#define _ERROR "Error"

namespace Engine {
    inline void SYSTEM(std::string className, std::string desc, std::string value) {
        _log::LogQueue& mLogger = _log::LogQueue::getInstance();
        mLogger.push(className, value, _SYS, desc);
    }

    inline void INFO(std::string className, std::string desc, std::string value) {
        _log::LogQueue& mLogger = _log::LogQueue::getInstance();
        mLogger.push(className, value, _INFO, desc);
    }

    inline void DEBUG(std::string className, std::string desc, std::string value) {
        _log::LogQueue& mLogger = _log::LogQueue::getInstance();
        mLogger.push(className, value, _DEBUG, desc);
    }
    inline void WARNING(std::string className, std::string desc, std::string value) {
        _log::LogQueue& mLogger = _log::LogQueue::getInstance();
        mLogger.push(className, value, _WARNING, desc);
    }
    inline void ERROR(std::string className, std::string desc, std::string value) {
        _log::LogQueue& mLogger = _log::LogQueue::getInstance();
        mLogger.push(className, value, _ERROR, desc);
    }
}
#endif