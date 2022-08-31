#ifndef MANAGER
#define MANAGER

#include "gtkmm/window.h"
#include "../interface/ihandler.hpp"
#include "info.hpp"
#include <stack>

namespace view
{
    class Manager : public worker::IHandler, public IWindow
    {
        private:
            std::stack<Gtk::Window*> _stack;
            Glib::RefPtr<Gtk::Application> _app;
            worker::IHandler* _handler;
            Info _info;
        public:
            Manager(worker::IHandler* handler);
            void add(Gtk::Window*);
            void run(Glib::RefPtr<Gtk::Application>);
            void event(view::IWindow*, std::unordered_map<std::string, std::string> data, handle type) override;
            void event(view::IWindow*, handle) override;

            void add_columns(std::vector<std::string> columns) override;
            void add_row(std::vector<std::string> row) override;
            void add_row(std::unordered_map<std::string,std::string> row) override;
            void add_rows(std::vector<std::vector<std::string>> rows) override;
            void show_message(std::string message) override;
    };
};

#endif //MANAGER
