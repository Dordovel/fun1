#include "header/connection.hpp"
#include "resultset_metadata.h"
#include <mysql/jdbc.h>
#include <iostream>
#include <stdlib.h>
#include "header/connection_settings.hpp"
#include "header/connection.hpp"

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include "header/window.hpp"


int main(void)
{
    connection::ConnectionSettings settings;

    connection::MysqlConnection& connection = connection::MysqlConnection::instance(settings);
    try
    {
        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("View");
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
        builder->add_from_file("./Main.glade");
        view::View* view = nullptr;
        builder->get_widget_derived("MainWindow", view);
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
