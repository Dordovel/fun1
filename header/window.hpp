#ifndef VIEW
#define VIEW

#include "glibmm/dispatcher.h"
#include "gtkmm/builder.h"
#include "gtkmm/combobox.h"
#include "gtkmm/liststore.h"
#include "gtkmm/treeview.h"
#include <gtkmm/window.h>
#include "../interface/itimer.hpp"

namespace view
{
    class View: public Gtk::Window
    {
        private:
            worker::IUpdateTimer* _timerSubscriber;

            Gtk::ComboBox* _timerBox = nullptr;

            Gtk::TreeView* _view = nullptr;
            Glib::RefPtr<Gtk::ListStore> _treeModel;
            Gtk::TreeModel::ColumnRecord _record;
            std::vector<Gtk::TreeModelColumn<std::string>> _columns;

            Glib::Dispatcher _dispatcher;
            std::vector<std::vector<std::string>> _workerBuffer;

            std::vector<long> _timers;

            void update();

            void init_timer();

            void signal_change_timer();

        public:
            View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade);
            void add_columns(std::string columns);
            void add_row(std::vector<std::string> row);
            void add_rows(std::vector<std::vector<std::string>> rows);
            void change_timer_subscribe(worker::IUpdateTimer* updater);
            void add_timer_value(long timer);
    };
};

#endif // VIEW
