#ifndef DELAY_ABSTRACT
#define DELAY_ABSTRACT

#include <atomic>
#include <thread>
#include <condition_variable>

namespace worker
{
    class ADelay
    {
        private:
            std::atomic<bool> _isRun;
            std::atomic<bool> _isPause;
            std::thread _thread;
            long _delay;
            std::condition_variable _cv;

        protected:

            virtual void invoke() const noexcept = 0;

            ADelay(long delay, bool stat);

            virtual ~ADelay();

            void worker(ADelay* delay) const noexcept;

    public:

            bool is_run() const noexcept;

            void run() noexcept;

            void stop() noexcept;
    };
};
#endif //DELAY_ABSTRACT
