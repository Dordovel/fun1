#ifndef CONNECTION_SETTINGS
#define CONNECTION_SETTINGS

#include <string>
#include <algorithm>
#include <cstring>

namespace connection
{
    class ConnectionSettings
    {
        private:

            std::string replace_protocol(std::string connectionTemplate)
            {
                auto pos = connectionTemplate.find("${protocol}");
                return connectionTemplate.replace(pos, strlen("${protocol}") , this->protocol);
            }

            std::string replace_url(std::string connectionTemplate)
            {
                auto pos = connectionTemplate.find("${url}");
                return connectionTemplate.replace(pos, strlen("${url}") , this->url);
            }

            std::string replace_port(std::string connectionTemplate)
            {
                auto pos = connectionTemplate.find("${port}");
                return connectionTemplate.replace(pos, strlen("${port}") , this->port);
            }

        public:
            static constexpr const char* url = "localhost";
            static constexpr const char* protocol = "tcp";
            static constexpr const char* port = "3306";
            static constexpr const char* user = "root";
            static constexpr const char* password = "root";
            static constexpr const char* schema = "";

            std::string create_by_template(std::string connectionTemplate)
            {
                return 
                            this->replace_port(
                                                this->replace_url(
                                                                    this->replace_protocol(connectionTemplate)
                                                                )
                                            );
            }
    };
};

#endif //CONNECTION_SETTINGS
