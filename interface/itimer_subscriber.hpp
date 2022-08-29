#ifndef ITIMERUPDATER
#define ITIMERUPDATER

#include "itimer.hpp"
#include <string>
#include <vector>

namespace worker
{
    class ITimerUpdater
    {
        public:
           virtual void timer_subscribe(worker::ITimer* updater) = 0;

            virtual ~ITimerUpdater() = default;
    };
};

#endif //ITIMERUPDATER
