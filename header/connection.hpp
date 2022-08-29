#ifndef CONNECTION
#define CONNECTION

#include "connection_settings.hpp"
#include <string>
#include <unordered_map>
#include <vector>

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
            sql::Connection* _connection;
            sql::Statement* _statement;
            sql::ResultSet* _resultSet;

            MysqlConnection(const connection::ConnectionSettings& settings);

        public:
            static MysqlConnection* instance(const connection::ConnectionSettings& settings);
            MysqlConnection(const MysqlConnection&) = delete;
            MysqlConnection& operator = (const MysqlConnection&) = delete;

            MysqlConnection(MysqlConnection&&);
            MysqlConnection& operator = (MysqlConnection&&);

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
