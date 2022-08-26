#ifndef TIMER_MODEL
#define TIMER_MODEL

#include <gtkmm/treemodel.h>

namespace view
{
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
        public:
            ModelColumns()
            { add(col_name); }

            Gtk::TreeModelColumn<Glib::ustring> col_name;
    };
};

#endif //TIMER_MODEL
