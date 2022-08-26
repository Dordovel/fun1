#ifndef TIMER_INTERFACE
#define TIMER_INTERFACE

namespace worker
{
    class IUpdateTimer
    {
        public:
            virtual void change_delay(long delay) = 0;
            virtual ~IUpdateTimer() = default;
    };
};

#endif //TIMER_INTERFACE
