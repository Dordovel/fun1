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

            virtual void invoke() noexcept = 0;

            ADelay(long delay);

            virtual ~ADelay();

            void worker(ADelay* delay) const noexcept;

    public:
            bool is_run() const noexcept;

            void run() noexcept;

            void stop() noexcept;

            void change_time(long delay);
    };
};
#endif //DELAY_ABSTRACT
