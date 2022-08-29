#include "../header/controller.hpp"
#include <iostream>

namespace worker
{
    Controller::Controller() : ADelay(1) {}

    std::vector<std::vector<std::string>> Controller::fetch_process_list_from_connection()
    {
        std::vector<std::vector<std::string>> buffer;
        this->_connections->process_list();
        buffer.reserve(this->_connections->fetch_size());

        while(this->_connections->next())
        {
            buffer.emplace_back(this->_connections->fetch_array());
        }
        this->_connections->clear_last_execute_decriptors();
        return buffer;
    }

    void Controller::invoke() noexcept
    {
        this->_update->update();
    }

    void Controller::event(view::IWindow* window, std::unordered_map<std::string, std::string> data, handle type)
    {
        switch (type)
        {
            case worker::IHandler::handle::KILL:
                this->_connections->kill_process(std::move(data));
            break;

            case worker::IHandler::handle::AUTH:
                 connection::ConnectionSettings settings;
                    settings.user = data["login"];
                    settings.password = data["password"];
                    this->_connections = connection::MysqlConnection::instance(settings);
            break;
        }
    }

    void Controller::event(view::IWindow* window, handle type)
    {
        switch (type)
        {
            case worker::IHandler::handle::COLUMNS:
            {
                this->_connections->process_list();
                auto columns = this->_connections->fetch_columns();
                for(auto&& column : columns)
                {
                    window->add_columns(std::move(column));
                }
                this->_connections->clear_last_execute_decriptors();
            }
            break;
            case worker::IHandler::handle::ROWS:
            {
                window->add_rows(this->fetch_process_list_from_connection());
            }
            break;
        }
    }

    void Controller::change_delay(long delay)
    {
        ADelay::change_time(delay);
    }

    void Controller::subscribe(IUpdate* update)
    {
        this->_update = update;
        ADelay::run();
    }

    void Controller::unsubscribe(IUpdate* update)
    {
        this->_update = nullptr;
        ADelay::stop();
    }
};
