#include "../header/info.hpp"
#include "gtkmm/enums.h"
#include "gtkmm/image.h"
#include "gtkmm/messagedialog.h"
#include "gtkmm/object.h"

namespace view
{
    Info::Info() : Gtk::MessageDialog("Info")
    {
        Gtk::MessageDialog::set_modal(true);
        this->_image = Gtk::manage(new Gtk::Image);
        this->_image->set_from_icon_name("dialog-warning", Gtk::ICON_SIZE_DIALOG);
        Gtk::MessageDialog::set_image(*this->_image);
        Gtk::MessageDialog::show_all_children();
        Gtk::MessageDialog::signal_response().connect(
            sigc::hide(sigc::mem_fun(this, &Gtk::Widget::hide)));
    }
    
    void Info::event_subscribe(worker::IHandler* handler)
    {
        this->_subscriber = handler;
    }

    void Info::show_message(std::string message)
    {
        Gtk::MessageDialog::set_secondary_text(message);
    }
};
