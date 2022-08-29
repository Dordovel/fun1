#ifndef IWINDOW
#define IWINDOW

namespace view
{
    class IWindow
    {
        public:
            virtual void add_columns(std::string columns) = 0;
            virtual void add_row(std::vector<std::string> row) = 0;
            virtual void add_rows(std::vector<std::vector<std::string>> rows) = 0;
            virtual ~IWindow() = default;
    };
};

#endif //IWINDOW
