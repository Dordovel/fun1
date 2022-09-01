#include "header/controller.hpp"
#include "header/info.hpp"
#include "header/login.hpp"
#include "header/manager.hpp"
#include "interface/itimer.hpp"
#include "interface/itimer_subscriber.hpp"
#include <iostream>
#include <iterator>
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

    std::vector<std::string> settingLabel = {"host", "port", "user", "password"};
    Glib::RefPtr<Gtk::Builder> builderLogin = Gtk::Builder::create();
    view::Login* login = nullptr;
    builderLogin->add_from_file("./Login.glade");
    builderLogin->get_widget_derived("LoginWindow", login);
    login->add_columns(settingLabel);

    libconfig::Config config;
    config.readFile("../config.ini");
    const libconfig::Setting& root = config.getRoot();
    const libconfig::Setting& settings = root["application"]["login"];
    const auto settingsCount = settings.getLength();
    for(std::decay_t<decltype(settingsCount)> i = 0; i < settingsCount; ++i)
    {
        std::vector<std::string> settingValue;
        settingValue.resize(settingLabel.size());

        const libconfig::Setting& setting = settings[i];
        for(decltype(settingLabel)::size_type g = 0; g < settingLabel.size(); ++g)
        {
            setting.lookupValue(settingLabel[g], settingValue[g]);
        }
        login->add_row(std::move(settingValue));
    }
    
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
