#include "header/connection.hpp"
#include "header/delay_base.hpp"
#include "resultset_metadata.h"
#include <mysql/jdbc.h>
#include <iostream>
#include <stdlib.h>
#include "header/connection_settings.hpp"
#include "header/connection.hpp"

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include "header/window.hpp"

#include "header/delay.hpp"
#include "header/worker.hpp"

int main(void)
{
    connection::ConnectionSettings settings;

    try
    {
        connection::MysqlConnection& connection = connection::MysqlConnection::instance(settings);

        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("View");
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
        builder->add_from_file("./Main.glade");
        view::View* view = nullptr;

        builder->get_widget_derived("MainWindow", view);

        if(!view) return EXIT_FAILURE;

        connection.process_list();
        auto columns = connection.fetch_columns();
        for(const std::string& column : columns)
        {
            view->add_columns(column);
        }

        while(connection.next())
        {
            view->add_row(connection.fetch_array());
        }

        auto functor = [&connection]()
            {
                std::vector<std::vector<std::string>> buffer;
                connection.process_list();
                buffer.reserve(connection.fetch_size());

                while(connection.next())
                {
                    buffer.emplace_back(connection.fetch_array());
                }
                connection.clear_last_execute_decriptors();
                return buffer;
            };

        auto callback = [&view](std::vector<std::vector<std::string>> rows)
            {
                view->add_rows(std::move(rows));
            };

        worker::Worker<decltype(callback) , decltype(functor)> worker(5, callback, functor);
        view->add_timer_value(5);
        view->add_timer_value(10);
        view->add_timer_value(20);
        view->add_timer_value(50);
        view->change_timer_subscribe(&worker);

        app->run(*view);
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
