#ifndef VIEW
#define VIEW

#include "gtkmm/builder.h"
#include "gtkmm/liststore.h"
#include "gtkmm/treeview.h"
#include <gtkmm/window.h>

namespace view
{
    class View: public Gtk::Window
    {
        private:
            Gtk::TreeView* _view = nullptr;
            Glib::RefPtr<Gtk::ListStore> _treeModel;
            Gtk::TreeModel::ColumnRecord _record;
            std::vector<Gtk::TreeModelColumn<std::string>> _columns;

        public:
            View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade);
            void add_columns(std::string columns);
    };
};

#endif // VIEW
