#ifndef MANAGER
#define MANAGER

#include "gtkmm/window.h"
#include "../interface/ihandler.hpp"
#include <stack>

namespace view
{
    class Manager : public worker::IHandler
    {
        private:
            std::stack<Gtk::Window*> _stack;
            Glib::RefPtr<Gtk::Application> _app;
            worker::IHandler* _handler;
        public:
            Manager(worker::IHandler* handler);
            void add(Gtk::Window*);
            void run(Glib::RefPtr<Gtk::Application>);
            void event(view::IWindow*, std::unordered_map<std::string, std::string> data, handle type) override;
            void event(view::IWindow*, handle) override;
    };
};

#endif //MANAGER
