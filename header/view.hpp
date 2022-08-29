#ifndef VIEW
#define VIEW

#include "glibmm/dispatcher.h"
#include "gtkmm/builder.h"
#include "gtkmm/button.h"
#include "gtkmm/combobox.h"
#include "gtkmm/liststore.h"
#include "gtkmm/treeview.h"
#include <gtkmm/window.h>
#include "../interface/itimer.hpp"
#include "../interface/iview.hpp"
#include "../interface/ihandler.hpp"

namespace view
{
    class View: public Gtk::Window, public IView 
    {
        private:
            worker::ITimer* _timerSubscriber;
            worker::IHandler* _subscriber;

            Gtk::ComboBox* _timerBox = nullptr;

            Gtk::TreeView* _view = nullptr;
            Glib::RefPtr<Gtk::ListStore> _treeModel;
            Gtk::TreeModel::ColumnRecord _record;
            std::vector<Gtk::TreeModelColumn<std::string>> _columns;

            Gtk::Button* _killButton = nullptr;

            Glib::Dispatcher _dispatcher;
            std::vector<std::vector<std::string>> _workerBuffer;

            std::vector<long> _timers;

            void refresh();

            void init_timer();

            void signal_change_timer();
            void signal_button_kill();
            void signal_change_item(const Gtk::TreePath &, Gtk::TreeViewColumn *);

            void signal_show();
            void signal_hide();

        public:
            View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade);
            void add_columns(std::string columns) override;
            void add_row(std::vector<std::string> row) override;
            void add_rows(std::vector<std::vector<std::string>> rows) override;
            void timer_subscribe(worker::ITimer* updater) override;
            void event_subscribe(worker::IHandler* handler) override;
            void add_timer_value(long timer);
            void update() override;
    };
};

#endif // VIEW
