#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <string>

class Controller {
    private:
    public:
        Controller () {}
        virtual ~Controller() {}
        virtual const std::string& getTag() = 0;
        virtual std::string getDesc() = 0;
        virtual std::string getStatus() = 0;
        virtual int getStatusInt() = 0;
        virtual int startThread() = 0;
        virtual bool getAutoRestartFlag() = 0;
};
#endif