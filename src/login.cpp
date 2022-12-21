#include "../header/login.hpp"
#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
#include "gtkmm/object.h"
#include "gtkmm/liststore.h"
#include "sigc++/functors/mem_fun.h"
#include <algorithm>
#include <bits/types/FILE.h>
#include <cstdio>
#include <iostream>
#include <iterator>
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

        if(this->read_config(this->_configPath))
            this->load_config();
    }

    void Login::signal_login_button_event()
    {
        std::string password = this->_passwordEntry->get_text();
        std::string login = this->_loginEntry->get_text();
        std::string host = this->_hostEntry->get_text();
        std::string port = this->_portEntry->get_text();

        libconfig::Setting& settings = this->get_history_node();
        libconfig::Setting& newNode = settings.add(libconfig::Setting::TypeGroup);
        newNode.add("user", libconfig::Setting::TypeString);
        newNode.add("password", libconfig::Setting::TypeString);
        newNode.add("host", libconfig::Setting::TypeString);
        newNode.add("port", libconfig::Setting::TypeString);

        newNode["user"] = login;
        newNode["password"] = password;
        newNode["host"] = host;
        newNode["port"] = port;

        this->write_config(this->_configPath);
        this->load_from_node(settings);

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
        auto path = selection->get_model()->get_path(iterator);
        int index = path[0];

        if(this->_rows.size() < index) return;
        decltype(this->_rows)::value_type row = this->_rows[index];
        
        for(const auto& value : row)
        {   
            if(iterator)
            {
                std::string title = value.first;
                if(title == "host")
                {
                    this->_hostEntry->set_text(value.second);
                }
                else if(title == "port")
                {
                    this->_portEntry->set_text(value.second);
                }
                else if(title == "user")
                {
                    this->_loginEntry->set_text(value.second);
                }
                else if(title == "password")
                {
                    this->_passwordEntry->set_text(value.second);
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

	void Login::hide()
	{
		Gtk::Window::hide();
	}

    std::array<std::pair<std::string, bool>, 4> Login::get_config_keys() const noexcept
    {
        return {
                std::make_pair("host", true), 
                std::make_pair("port", true),
                std::make_pair("user", true),
                std::make_pair("password", false)
        };
    }

    bool Login::read_config(const std::string& path)
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

                bool read(libconfig::Config& config)
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

        rai config(path);
        return config.read(this->_config);
    }

    bool Login::write_config(const std::string& path)
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

                bool read(libconfig::Config& config)
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

        rai config(path);
        return config.read(this->_config);
    }

    void Login::clear_history()
    {
        this->_rows.clear();
        this->_columns.clear();
        this->_historyBox->remove_all_columns();
    }

    libconfig::Setting& Login::get_history_node()
    {
        libconfig::Setting& root = this->_config.getRoot();
        if(!root.exists("application"))
        {
            libconfig::Setting& appNode = root.add("application", libconfig::Setting::TypeGroup);
            libconfig::Setting& historyNode = appNode.add("history", libconfig::Setting::TypeList);

            return historyNode;
        }

        libconfig::Setting& settings = root["application"]["history"];
        return settings;
    }

    void Login::load_from_node(const libconfig::Setting& settings)
    {
        this->clear_history();

        const auto settingsCount = settings.getLength();

        std::array<std::pair<std::string, bool>, 4> keys = this->get_config_keys();
        std::unordered_map<std::string, std::string> values = {};

        std::vector<std::string> row = {};

        for(const auto& key : keys)
        {
            if(key.second) this->add_column(key.first);
        }

        for(int i = 0; i < settingsCount; ++i)
        {
            values.reserve(keys.size());
            row.reserve(keys.size());

            if(settings[i].isGroup())
            {
                libconfig::Setting& setting = settings[i];
                for(const auto& key : keys)
                {
                    std::string buffer;
                    setting.lookupValue(key.first, buffer);
                    values.emplace(key.first, buffer);

                    if(key.second) row.push_back(std::move(buffer));
                }
            }

            this->add_row(std::move(row));
            this->_rows.push_back(std::move(values));
        }
    }

    void Login::load_config()
    {
        const libconfig::Setting& settings = this->get_history_node();
        this->load_from_node(settings);
    }
};
