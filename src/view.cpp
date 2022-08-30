#include "../header/view.hpp"
#include "../header/update_view_timer_model.hpp"
#include "gtkmm/treeselection.h"
#include "sigc++/functors/mem_fun.h"
#include <glibmm-2.4/glibmm/refptr.h>
#include <string>
#include <unordered_map>

namespace view
{
    View::View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade) : Gtk::Window(cobject)
    {
        m_RefGlade->get_widget("Table", this->_view);
        m_RefGlade->get_widget("Timer", this->_timerBox);
        m_RefGlade->get_widget("KillButton", this->_killButton);

        Gtk::Window::signal_show().connect(sigc::mem_fun(this, &View::signal_show));
        Gtk::Window::signal_hide().connect(sigc::mem_fun(this, &View::signal_hide));

        this->_killButton->signal_clicked().connect(sigc::mem_fun(this, &View::signal_button_kill));
        this->_view->signal_row_activated().connect(sigc::mem_fun(this, &View::signal_change_item));

        this->_timerBox->signal_changed().connect(sigc::mem_fun(this, &View::signal_change_timer));
        this->_dispatcher.connect(sigc::mem_fun(this, &View::refresh));
    }

    void View::init_timer()
    {
        if(this->_timerBox)
        {
            view::ModelColumns model;
            Glib::RefPtr<Gtk::ListStore> treeModel;
            treeModel = Gtk::ListStore::create(model);
            this->_timerBox->set_model(treeModel);

            for(const auto& timer : this->_timers)
            {
                auto iter = treeModel->append();
                auto row = *iter;
                row[model.col_name] = "..." + std::to_string(timer) + " seconds";
            }
            this->_timerBox->pack_start(model.col_name);
            this->_timerBox->set_active(0);
            this->_timerBox->show_all_children();
        }
    }

    void View::signal_change_timer()
    {
        view::ModelColumns model;
        int activeId = this->_timerBox->get_active_row_number();
        if(0 < activeId)
        {
            if (this->_timerSubscriber) 
            {
                this->_timerSubscriber->change_delay(this->_timers[activeId]);
            }
        }
    }

    void View::signal_change_item(const Gtk::TreePath & tree, Gtk::TreeViewColumn * column)
    {
    }

    void View::signal_button_kill()
    {
        Glib::RefPtr<Gtk::TreeSelection> selection = this->_view->get_selection();
        Gtk::TreeModel::iterator iterator = selection->get_selected();
        std::unordered_map<std::string, std::string> columns;
        columns.reserve(this->_columns.size());
        
        for(decltype(this->_columns)::size_type i = 0; i < this->_columns.size(); ++i)
        {   
            auto& column = this->_columns[i];
            auto* header = this->_view->get_column(i);

            if(iterator)
            {
                std::string value = iterator->get_value(column);
                columns.emplace(header->get_title(), value);
            }
        }
        
        this->_subscriber->event(static_cast<IWindow*>(this), columns, worker::IHandler::handle::KILL);
    
    }

    void View::signal_show()
    {
        this->_subscriber->event(static_cast<IWindow*>(this), worker::IHandler::handle::COLUMNS);
        this->_timerSubscriber->subscribe(static_cast<worker::IUpdate*>(this));
    }

    void View::signal_hide()
    {
        this->_timerSubscriber->unsubscribe(static_cast<worker::IUpdate*>(this));
    }

    void View::refresh()
    {
        this->_treeModel->clear();

        for(auto&& row : this->_workerBuffer)
        {
            this->add_row(std::move(row));
        }

        this->_workerBuffer.clear();
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

    void View::add_rows(std::vector<std::vector<std::string>> rows)
    {
        if(this->_workerBuffer.empty())
        {
            this->_workerBuffer = std::move(rows);
            this->_dispatcher.emit();
        }
    }

    void View::show_message(std::string message)
    {
        
    }

    void View::timer_subscribe(worker::ITimer* updater)
    {
        this->_timerSubscriber = updater;
        this->init_timer();
    }

    void View::event_subscribe(worker::IHandler* handler)
    {
        this->_subscriber = handler;
    }

    void View::add_timer_value(long timer)
    {
        this->_timers.push_back(std::move(timer));
    }

    void View::update()
    {
        this->_subscriber->event(static_cast<IWindow*>(this), worker::IHandler::handle::ROWS);
    }
};
