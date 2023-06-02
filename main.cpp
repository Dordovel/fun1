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
#include <unordered_map>

const inline std::string configPath = "./history";

bool read_config_from_file(libconfig::Config& config, const std::string& path)
{
    class rai
    {
        private:
            FILE* file;
        public:
            rai(const std::string& path)
            {
                file = fopen(path.c_str(), "r");
            }

            bool read_in_config(libconfig::Config& config)
            {
                if(NULL == file) return false;
                config.read(file);
                return true;
            }
            
            ~rai() noexcept
            {
                if(file != NULL)
                    fclose(file);
            }
    };

    rai configReader(path);
    return configReader.read_in_config(config);
}

bool flush_config_to_file(libconfig::Config& config, const std::string& path)
{
    class rai
    {
        private:
            FILE* file;
        public:
            rai(const std::string& path)
            {
                file = fopen(path.c_str(), "w");
            }

            bool write_config(libconfig::Config& config)
            {
                if(NULL == file) return false;
                config.write(file);
                return true;
            }
            
            ~rai() noexcept
            {
                if(file != NULL)
                    fclose(file);
            }
    };

    rai configWriter(path);
    return configWriter.write_config(config);
}

libconfig::Setting& get_history_node_from_config(libconfig::Config& config)
{
    libconfig::Setting& root = config.getRoot();
    if(!root.exists("application"))
    {
        libconfig::Setting& appNode = root.add("application", libconfig::Setting::TypeGroup);
        libconfig::Setting& historyNode = appNode.add("history", libconfig::Setting::TypeList);

        return historyNode;
    }

    libconfig::Setting& settings = root["application"]["history"];
    return settings;
}

int main(void)
{
    worker::Controller controller;

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("View");
    Glib::RefPtr<Gtk::Builder> builderView = Gtk::Builder::create();

    view::View* view = nullptr;
    builderView->add_from_file("./Main.glade");
    builderView->get_widget_derived("MainWindow", view);
    if(!view) return EXIT_FAILURE;

    libconfig::Config config;
    read_config_from_file(config, configPath);

    Glib::RefPtr<Gtk::Builder> builderLogin = Gtk::Builder::create();
    libconfig::Setting& loginConfig = get_history_node_from_config(config);
    view::Login* login = nullptr;
    builderLogin->add_from_file("./Login.glade");
    builderLogin->get_widget_derived("LoginWindow", login, loginConfig);
    
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

    flush_config_to_file(config, configPath);

    return EXIT_SUCCESS;
}
