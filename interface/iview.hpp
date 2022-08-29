#ifndef IVIEW
#define IVIEW

#include "ievent_subscriber.hpp"
#include "itimer.hpp"
#include "iwindow.hpp"
#include "itimer_subscriber.hpp"

namespace view
{
    class IView : public worker::ITimerUpdater, public worker::IEvent, public worker::IUpdate, public IWindow
    {
        public:
            virtual ~IView() = default;
    };
};

#endif //IVIEW
