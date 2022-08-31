#include "header/controller.hpp"
#include "header/info.hpp"
#include "header/login.hpp"
#include "header/manager.hpp"
#include "interface/itimer.hpp"
#include "interface/itimer_subscriber.hpp"
#include <iostream>
#include <stdlib.h>

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include "header/view.hpp"

#include <libconfig.h++>
#include <type_traits>

int main(void)
{
    worker::Controller controller;

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("View");
    Glib::RefPtr<Gtk::Builder> builderView = Gtk::Builder::create();

    view::View* view = nullptr;
    builderView->add_from_file("./Main.glade");
    builderView->get_widget_derived("MainWindow", view);
    if(!view) return EXIT_FAILURE;

    Glib::RefPtr<Gtk::Builder> builderLogin = Gtk::Builder::create();
    view::Login* login = nullptr;
    builderLogin->add_from_file("./Login.glade");
    builderLogin->get_widget_derived("LoginWindow", login);

    view::Manager manager(static_cast<worker::IHandler*>(&controller));

    view->add_timer_value(1);
    view->add_timer_value(5);
    view->add_timer_value(10);
    view->add_timer_value(20);
    view->add_timer_value(50);
    view->set_size_request(900, 500);
    view->event_subscribe(static_cast<worker::IHandler*>(&manager));
    view->timer_subscribe(static_cast<worker::ITimer*>(&controller));

    login->event_subscribe(static_cast<worker::IHandler*>(&manager));

    manager.add(view);
    manager.add(login);
    manager.run(app);
    return EXIT_SUCCESS;
}
