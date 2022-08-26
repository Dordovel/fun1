#ifndef WORKER
#define WORKER

#include "delay.hpp"
#include "../interface/itimer.hpp"

namespace worker
{
    template<typename Callback, typename Functor>
    class Worker : private worker::Delay<Callback, Functor>, public worker::IUpdateTimer
    {
        public:
            Worker(long delay, Callback callback, Functor functor) : worker::Delay<Callback, Functor>(delay, callback,                                                                                           functor)
            {}

            void change_delay(long delay) override
            {
                worker::Delay<Callback, Functor>::change_time(delay);
            }
    };
};

#endif //WORKER
