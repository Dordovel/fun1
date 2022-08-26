#ifndef DELAY_IMPL
#define DELAY_IMPL

#include "./delay_base.hpp"
#include "./invoke.hpp"

namespace worker
{
    template <typename Callback, typename Functor>
    class Delay : public ADelay
    {
        private:
            Callback _callback;
            Functor _functor;

            void invoke() const noexcept override
            {
                worker::invoke(this->_callback, worker::invoke(this->_functor));
            }

        public:
            Delay(long delay, Callback callback, Functor functor):ADelay(delay, false),
                                                                _callback(callback),
                                                                _functor(functor) {}

    };
}
#endif // DELAY_IMPL
