#include "../header/delay_base.hpp"

namespace worker
{
    ADelay::ADelay(long delay, bool stat): _isRun(stat), _isPause(stat), _delay(delay)
    {
        this->_isRun = true;
        this->_thread = std::thread(&ADelay::worker, this, this);
    }

    bool ADelay::is_run() const noexcept
    {
        return this->_isPause;
    }

    void ADelay::run() noexcept
    {
        this->_isPause = false;
        this->_cv.notify_one();
    }

    void ADelay::worker(ADelay* delay) const noexcept
    {
        std::mutex mt;
        std::unique_lock<std::mutex> lk(mt);

        while(delay->_isRun)
        {
            if(!delay->_isPause)
            {
                delay->invoke();
                std::this_thread::sleep_for(std::chrono::seconds(delay->_delay));
            }
            else if(delay->_isPause)
            {
                delay->_cv.wait(lk);
            }
        }
    }

    void ADelay::stop() noexcept
    {
        this->_isPause = true;
    }

    void ADelay::change_time(long delay)
    {
        this->_delay = delay;
    }

    ADelay::~ADelay()
    {
        if(this->_isRun)
            this->_isRun = false;

        if(this->_isPause)
        {
            this->_isPause = false;
            this->_cv.notify_one();
        }

        if(this->_thread.joinable())
            this->_thread.join();
    }
};
