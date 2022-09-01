#include "../header/login.hpp"
#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
#include "gtkmm/object.h"
#include "gtkmm/liststore.h"
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
        m_RefGlade->get_widget("Port", this->_portEntry);
        m_RefGlade->get_widget("Host", this->_hostEntry);
        m_RefGlade->get_widget("History", this->_historyBox);

        this->_historyBox->signal_row_activated().connect(sigc::mem_fun(this, &Login::signal_row_activate));

        this->_applyButton->signal_clicked().connect(sigc::mem_fun(this, &Login::signal_login_button_event));
    }

    void Login::signal_login_button_event()
    {
        std::string password = this->_passwordEntry->get_text();
        std::string login = this->_loginEntry->get_text();
        std::string host = this->_hostEntry->get_text();
        std::string port = this->_portEntry->get_text();

        std::unordered_map<std::string, std::string>data;
        data.emplace("login", std::move(login));
        data.emplace("password", std::move(password));
        data.emplace("host", std::move(host));
        data.emplace("port", std::move(port));
        this->_subscriber->event(static_cast<IWindow*>(this), data, worker::IHandler::handle::AUTH);
        this->_subscriber->event(static_cast<IWindow*>(this), worker::IHandler::handle::HIDE);
    }

    void Login::signal_row_activate(const Gtk::TreePath &, Gtk::TreeViewColumn *)
    {
        Glib::RefPtr<Gtk::TreeSelection> selection = this->_historyBox->get_selection();
        Gtk::TreeModel::iterator iterator = selection->get_selected();
        
        for(decltype(this->_columns)::size_type i = 0; i < this->_columns.size(); ++i)
        {   
            auto& column = this->_columns[i];
            auto* header = this->_historyBox->get_column(i);

            if(iterator)
            {
                std::string value = iterator->get_value(column);
                std::string title = header->get_title();
                if(title == "host")
                {
                    this->_hostEntry->set_text(value);
                }
                else if(title == "port")
                {
                    this->_portEntry->set_text(value);
                }
                else if(title == "user")
                {
                    this->_loginEntry->set_text(value);
                }
                else if(title == "password")
                {
                    this->_passwordEntry->set_text(value);
                }
            }
        }
    }

    void Login::event_subscribe(worker::IHandler* handler)
    {
        this->_subscriber = handler;
    }

    void Login::add_column(std::string column)
    {
        Gtk::TreeModelColumn<std::string> columnModel;
        this->_record.add(columnModel);
        this->_treeModel = Gtk::ListStore::create(this->_record);
        this->_historyBox->set_model(this->_treeModel);

        this->_historyBox->append_column(column, columnModel);
        this->_columns.push_back(std::move(columnModel));
        this->_historyBox->show_all_children();
    }

    void Login::add_columns(std::vector<std::string> columns)
    {
        this->_columns.clear();
        this->_columns.reserve(columns.size());

        for(auto begin = columns.begin(); begin < columns.end(); ++begin)
        {
            this->add_column(std::move(*begin));
        }
    }

    void Login::add_row(std::vector<std::string> row)
    {
        auto refRecord = *(this->_treeModel->append());

        for(decltype(this->_columns)::size_type i = 0; i < this->_columns.size(); ++i)
        {
            refRecord[this->_columns[i]] = row[i];
        }
    }

    void Login::add_rows(std::vector<std::vector<std::string>> rows){}
    void Login::show_message(std::string message){}

    std::vector<std::string> Login::get_setting_key()
    {
        return std::vector<std::string> {"host", "port", "user", "password"};
    }

    void Login::load_settings(std::unordered_map<std::string, std::string> settings)
    {
        if(this->_columns.empty())
        {
            this->add_columns(this->get_setting_key());
        }

        std::vector<std::string> row;
        row.reserve(this->_columns.size());
        
        std::vector<std::string>keys = this->get_setting_key();

        for(const std::string& key : keys)
        {
            row.emplace_back(std::move(settings[key]));
        }

        this->add_row(row);
    }

	void Login::hide()
	{
		Gtk::Window::hide();
	}
};
