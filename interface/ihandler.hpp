#ifndef IHANDLER
#define IHANDLER

#include "iwindow.hpp"
#include <string>
#include <unordered_map>
namespace worker
{
    class IHandler
    {
        public:
            enum class handle
            {
                AUTH,
                KILL,
                SHOW,
                HIDE,
                COLUMNS,
                ROWS
            };

            virtual void event(view::IWindow*, std::unordered_map<std::string, std::string>, handle) = 0;
            virtual void event(view::IWindow*, handle) = 0;
            virtual ~IHandler() = default;
    };
};

#endif //IHANDLER
