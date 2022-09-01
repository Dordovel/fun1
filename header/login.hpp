#ifndef LOGIN
#define LOGIN

#include <gtkmm/window.h>
#include "../interface/ihandler.hpp"
#include "../interface/isettings.hpp"
#include "../interface/iwindow.hpp"
#include "../interface/ievent_subscriber.hpp"
#include "gtkmm/builder.h"
#include "gtkmm/button.h"
#include "gtkmm/entry.h"
#include "gtkmm/liststore.h"
#include "gtkmm/treeview.h"

namespace view
{
    class Login: public Gtk::Window, public worker::IEvent, public IWindow, public settings::ISettings
    {
        private:
            worker::IHandler* _subscriber;
            Gtk::Entry* _loginEntry = nullptr;
            Gtk::Entry* _passwordEntry = nullptr;
			Gtk::Entry* _hostEntry = nullptr;
			Gtk::Entry* _portEntry = nullptr;
			Gtk::Button* _applyButton = nullptr;

            Gtk::TreeView* _historyBox = nullptr;
            Glib::RefPtr<Gtk::ListStore> _treeModel;
            Gtk::TreeModel::ColumnRecord _record;
            std::vector<Gtk::TreeModelColumn<std::string>> _columns;

            void signal_login_button_event();
            void signal_row_activate(const Gtk::TreePath &, Gtk::TreeViewColumn *);
            void add_column(std::string columns);
            
        public:
            Login(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade);
            void event_subscribe(worker::IHandler* handler) override;
            void add_columns(std::vector<std::string> columns) override;
            void add_row(std::vector<std::string> row) override;
            void add_rows(std::vector<std::vector<std::string>> rows) override;
            void show_message(std::string message) override;

            std::vector<std::string> get_setting_key() override;
            void load_settings(std::unordered_map<std::string, std::string>) override;
    };
};

#endif //LOGIN
