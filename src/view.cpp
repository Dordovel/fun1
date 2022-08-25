#include "../header/window.hpp"

namespace view
{
    View::View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade) : Gtk::Window(cobject)
    {
        m_RefGlade->get_widget("Table", this->_view);
    }

    void View::add_columns(std::string columns)
    {
        this->_columns.emplace_back();
        this->_record.add(this->_columns.back());
        this->_treeModel = Gtk::ListStore::create(this->_record);
    }
};
