#ifndef LOGIN
#define LOGIN

#include <gtkmm/window.h>
#include "../interface/ihandler.hpp"
#include "../interface/iwindow.hpp"
#include "../interface/ievent_subscriber.hpp"
#include "gtkmm/builder.h"
#include "gtkmm/button.h"
#include "gtkmm/entry.h"

namespace view
{
    class Login: public Gtk::Window, public worker::IEvent, public IWindow
    {
        private:
            worker::IHandler* _subscriber;
            
            Gtk::Entry* _loginEntry = nullptr;
            Gtk::Entry* _passwordEntry = nullptr;
            Gtk::Button* _applyButton = nullptr;

            void signal_login_button_event();
            
        public:
            Login(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade);
            void event_subscribe(worker::IHandler* handler) override;
            void add_columns(std::string columns) override;
            void add_row(std::vector<std::string> row) override;
            void add_rows(std::vector<std::vector<std::string>> rows) override;
    };
};

#endif //LOGIN
