#ifndef IWINDOW
#define IWINDOW

#include <string>
#include <unordered_map>
#include <vector>
namespace view
{
    class IWindow
    {
        public:
            virtual void add_columns(std::vector<std::string> columns) = 0;
            virtual void add_row(std::vector<std::string> row) = 0;
            virtual void add_rows(std::vector<std::vector<std::string>> rows) = 0;
            virtual void show_message(std::string message) = 0;
			virtual void hide() = 0;
            virtual ~IWindow() = default;
    };
};

#endif //IWINDOW
