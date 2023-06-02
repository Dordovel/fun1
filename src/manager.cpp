#include "../header/manager.hpp"

namespace view
{
    Manager::Manager(worker::IHandler* handler)
    {
        this->_handler = handler;
    }

    void Manager::add(Gtk::Window * window)
    {
        this->_stack.push(window);
    }

    void Manager::run(Glib::RefPtr<Gtk::Application> app)
    {
        this->_app = app;
        this->_app->run(*(this->_stack.top()));
    }

    void Manager::event(view::IWindow* window, std::unordered_map<std::string, std::string> data, handle type)
    {
        this->_handler->event(static_cast<IWindow*>(this), std::move(data), type);
    }

    void Manager::event(view::IWindow* window, handle type)
    {
		this->_handler->event(window, type);
    }

    void Manager::add_columns(std::vector<std::string> columns)
    {
        auto pointer = reinterpret_cast<IWindow*>(this->_stack.top());
        pointer->add_columns(std::move(columns));
    }

    void Manager::add_row(std::vector<std::string> row)
    {
        auto pointer = reinterpret_cast<IWindow*>(this->_stack.top());
        pointer->add_row(std::move(row));
    }

    void Manager::add_rows(std::vector<std::vector<std::string>> rows)
    {
        auto pointer = reinterpret_cast<IWindow*>(this->_stack.top());
        pointer->add_rows(std::move(rows));
    }

    void Manager::show_message(std::string message)
    {
        this->_info.show_message(std::move(message));
        this->_info.show();
    }

	void Manager::hide()
	{
		Gtk::Window* wnd = this->_stack.top();
		wnd->hide();
		this->_app->remove_window(*wnd);
		this->_stack.pop();
		this->_app->add_window(*(this->_stack.top()));
		this->_stack.top()->show();
	}
};
