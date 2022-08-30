#ifndef INFO
#define INFO

#include <gtkmm/messagedialog.h>
#include "../interface/ihandler.hpp"
#include "../interface/iwindow.hpp"
#include "../interface/ievent_subscriber.hpp"
#include "gtkmm/builder.h"
#include "gtkmm/button.h"
#include "gtkmm/entry.h"
#include "gtkmm/image.h"

namespace view
{
    class Info: public Gtk::MessageDialog, public worker::IEvent
    {
        private:
            worker::IHandler* _subscriber;
            Gtk::Image* _image;
            
        public:
            Info();
            void event_subscribe(worker::IHandler* handler) override;
            void show_message(std::string message);
    };
};

#endif //LOGIN
