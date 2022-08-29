#ifndef IEVENT
#define IEVENT

#include "ihandler.hpp"
namespace worker
{
    class IEvent
    {
        public:
            virtual void event_subscribe(worker::IHandler* handler) = 0;
            virtual ~IEvent() = default;
    };
};

#endif //IVENT
