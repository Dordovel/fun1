#ifndef CONNECTION
#define CONNECTION

#include "connection_settings.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <mariadb/conncpp/Driver.hpp>

namespace sql
{
    class Connection;
    class Statement;
    class ResultSet;
};

namespace connection
{
    class MysqlConnection
    {
        private:
            sql::Connection* _connection = nullptr;
            sql::Statement* _statement = nullptr;
            sql::ResultSet* _resultSet = nullptr;

            MysqlConnection(const connection::ConnectionSettings& settings);

        public:
            static MysqlConnection* instance(const connection::ConnectionSettings& settings);
            MysqlConnection(const MysqlConnection&) = delete;
            MysqlConnection& operator = (const MysqlConnection&) = delete;

            MysqlConnection(MysqlConnection&&) noexcept ;
            MysqlConnection& operator = (MysqlConnection&&) noexcept;

            std::size_t fetch_size() const;
            bool next();
            void execute(const std::string& query);
            std::unordered_map<std::string, std::string> fetch_assoc();
            std::vector<std::string> fetch_array();
            std::vector<std::string> fetch_columns();
            void clear_last_execute_decriptors();

            void process_list();
            void kill_process(std::unordered_map<std::string, std::string>);
            
            ~MysqlConnection();
    };
};

#endif //CONNECTION
