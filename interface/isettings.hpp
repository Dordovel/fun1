#ifndef ISETTINGS
#define ISETTINGS

#include <string>
#include <unordered_map>
#include <vector>
namespace settings
{
    class ISettings
    {
        public:
            virtual std::vector<std::string> get_setting_key() = 0;
            virtual void load_settings(std::unordered_map<std::string, std::string>) = 0;
            virtual ~ISettings() = default;
    };
};

#endif //ISETTINGS
