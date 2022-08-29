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
        this->_handler->event(window, std::move(data), type);
    }

    void Manager::event(view::IWindow* window, handle type)
    {
        switch (type)
        {
            case worker::IHandler::handle::HIDE:
            {
                Gtk::Window* wnd = this->_stack.top();
                this->_handler->event(window, type);
                wnd->hide();
                this->_app->remove_window(*wnd);
                this->_stack.pop();
                this->_app->add_window(*(this->_stack.top()));
                this->_stack.top()->show();
            }
            break;
            default:
                this->_handler->event(window, type);
            break;
        }
    }
};
