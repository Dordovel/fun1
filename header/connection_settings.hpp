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

            std::string replace_protocol(std::string connectionTemplate) const
            {
                auto pos = connectionTemplate.find("${protocol}");
                return connectionTemplate.replace(pos, strlen("${protocol}") , this->protocol);
            }

            std::string replace_url(std::string connectionTemplate) const
            {
                auto pos = connectionTemplate.find("${url}");
                return connectionTemplate.replace(pos, strlen("${url}") , this->url);
            }

            std::string replace_port(std::string connectionTemplate) const
            {
                auto pos = connectionTemplate.find("${port}");
                return connectionTemplate.replace(pos, strlen("${port}") , this->port);
            }

        public:
            std::string url = "localhost";
            std::string protocol = "tcp";
            std::string port = "3306";
            std::string user;
            std::string password;
            std::string schema;

            std::string create_by_template(std::string connectionTemplate) const
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
