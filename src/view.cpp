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
        this->_view->set_model(this->_treeModel);

		this->_view->append_column(columns, this->_columns.back());
        this->_view->show_all_children();
    }

    void View::add_row(std::vector<std::string> row)
    {
        auto refRecord = *(this->_treeModel->append());

        for(decltype(this->_columns)::size_type i = 0; i < this->_columns.size(); ++i)
        {
            refRecord[this->_columns[i]] = row[i];
        }
    }
};
