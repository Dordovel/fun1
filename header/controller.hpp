#ifndef CONTROLLER
#define CONTROLLER

#include "connection.hpp"
#include "../interface/ihandler.hpp"
#include "delay_base.hpp"
#include "login.hpp"
#include "view.hpp"

namespace worker
{
    class Controller : private ADelay, public ITimer, public IHandler
    {
        IUpdate* _update;
        connection::MysqlConnection* _connections = nullptr;

        std::vector<std::vector<std::string>> fetch_process_list_from_connection();

        public:
            Controller();
            void update();
            
            void invoke() noexcept override;
            void event(view::IWindow*, std::unordered_map<std::string, std::string> data, handle type) override;
            void event(view::IWindow*, handle) override;
            void change_delay(long delay) override;
            void subscribe(IUpdate*) override;
            void unsubscribe(IUpdate*) override;
    };
};

#endif //CONTROLLER
