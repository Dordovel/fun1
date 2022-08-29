#ifndef TIMER_INTERFACE
#define TIMER_INTERFACE

#include "iupdate.hpp"
namespace worker
{
    class ITimer
    {
        public:
            virtual void change_delay(long delay) = 0;
            virtual void subscribe(IUpdate*) = 0;
            virtual void unsubscribe(IUpdate*) = 0;
            virtual ~ITimer() = default;
    };
};

#endif //TIMER_INTERFACE
