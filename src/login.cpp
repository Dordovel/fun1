#include "../header/login.hpp"
#include "sigc++/functors/mem_fun.h"
#include <string>
#include <unordered_map>

namespace view
{
    Login::Login(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade):Gtk::Window(cobject)
    {
        m_RefGlade->get_widget("LoginEntry", this->_loginEntry);
        m_RefGlade->get_widget("PasswordEntry", this->_passwordEntry);
        m_RefGlade->get_widget("ButtonLogin", this->_applyButton);

        this->_applyButton->signal_clicked().connect(sigc::mem_fun(this, &Login::signal_login_button_event));
    }

    void Login::signal_login_button_event()
    {
        std::string password = this->_passwordEntry->get_text();
        std::string login = this->_loginEntry->get_text();
        std::unordered_map<std::string, std::string>data;
        data.emplace("login", std::move(login));
        data.emplace("password", std::move(password));
        this->_subscriber->event(static_cast<IWindow*>(this), data, worker::IHandler::handle::AUTH);
        this->_subscriber->event(static_cast<IWindow*>(this), worker::IHandler::handle::HIDE);
    }

    void Login::event_subscribe(worker::IHandler* handler)
    {
        this->_subscriber = handler;
    }

    void Login::add_columns(std::string columns){}
    void Login::add_row(std::vector<std::string> row){}
    void Login::add_rows(std::vector<std::vector<std::string>> rows){}
};
