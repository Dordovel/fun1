#include "../header/window.hpp"
#include "../header/update_view_timer_model.hpp"
#include "sigc++/functors/mem_fun.h"
#include <iostream>
#include <string>

namespace view
{
    View::View(Gtk::Window::BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& m_RefGlade) : Gtk::Window(cobject)
    {
        m_RefGlade->get_widget("Table", this->_view);
        m_RefGlade->get_widget("Timer", this->_timerBox);

        this->_timerBox->signal_changed().connect(sigc::mem_fun(this, &View::signal_change_timer));
        this->_dispatcher.connect(sigc::mem_fun(this, &View::update));
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

    void View::update()
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

    void View::change_timer_subscribe(worker::IUpdateTimer* updater)
    {
        this->_timerSubscriber = updater;
        this->init_timer();
    }

    void View::add_timer_value(long timer)
    {
        this->_timers.push_back(std::move(timer));
    }
};
