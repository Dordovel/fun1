#include "../header/connection.hpp"
#include "mariadb/conncpp/Driver.hpp"
#include "mariadb/conncpp/ResultSetMetaData.hpp"
#include "mariadb/conncpp/Statement.hpp"
#include <cstddef>
#include <iostream>

namespace connection
{
    MysqlConnection::MysqlConnection(const connection::ConnectionSettings& settings)
    {
        sql::Driver *driver = sql::mariadb::get_driver_instance();
        this->_connection = driver->connect(settings.create_by_template("${protocol}://${url}:${port}")
                              ,settings.user, settings.password);
        this->_statement = this->_connection->createStatement();
    }

    MysqlConnection* MysqlConnection::instance(const connection::ConnectionSettings& settings)
    {
        static MysqlConnection connection(settings);
        return &connection;
    }

    MysqlConnection::MysqlConnection(MysqlConnection&& oldConnection) noexcept
    {
        this->_connection = oldConnection._connection;
        this->_statement = oldConnection._statement;

        oldConnection._statement = nullptr;
        oldConnection._connection = nullptr;
    }

    MysqlConnection& MysqlConnection::operator = (MysqlConnection&& oldConnection) noexcept
    {
        this->_connection = oldConnection._connection;
        this->_statement = oldConnection._statement;

        oldConnection._statement = nullptr;
        oldConnection._connection = nullptr;

        return *this;
    }

    std::size_t MysqlConnection::fetch_size() const
    {
        return this->_resultSet->rowsCount();
    }

    bool MysqlConnection::next()
    {
        return this->_resultSet->next();
    }

    void MysqlConnection::execute(const std::string& query)
    {
        this->clear_last_execute_decriptors();
        this->_resultSet = this->_statement->executeQuery(query);
    }

    std::unordered_map<std::string, std::string> MysqlConnection::fetch_assoc()
    {
        std::unordered_map<std::string, std::string> result;

        sql::ResultSetMetaData* meta = this->_resultSet->getMetaData();

        const std::size_t fieldsCount = meta->getColumnCount();
        result.reserve(fieldsCount);
        
        for(std::size_t field = 1; field < fieldsCount; ++field)
        {
            result.emplace(meta->getColumnLabel(field), this->_resultSet->getString(field));
        }

        delete meta;

        return result;
    }

    std::vector<std::string> MysqlConnection::fetch_array()
    { 
        std::vector<std::string> result;

        sql::ResultSetMetaData* meta = this->_resultSet->getMetaData();

        const std::size_t fieldsCount = meta->getColumnCount();
        result.reserve(fieldsCount);
        
        for(std::size_t field = 1; field <= fieldsCount; ++field)
        {
            result.emplace_back(this->_resultSet->getString(field));
        }

        delete meta;

        return result;
    }

    std::vector<std::string> MysqlConnection::fetch_columns()
    {
        std::vector<std::string> result;
        sql::ResultSetMetaData* meta = this->_resultSet->getMetaData();
        const std::size_t fieldsCount = meta->getColumnCount();
        result.reserve(fieldsCount);

        for(std::size_t field = 1; field <= fieldsCount; ++field)
        {
            result.emplace_back(meta->getColumnLabel(field));
        }

        delete meta;

        return result;
    }

    void MysqlConnection::clear_last_execute_decriptors()
    {
        if(this->_resultSet)
        {
            if(!this->_resultSet->isClosed())
            {
                this->_resultSet->close();
            }

            delete this->_resultSet;
            this->_resultSet = nullptr;
        }
    }

    void MysqlConnection::process_list()
    {
        this->execute("SELECT * FROM information_schema.PROCESSLIST");
    }

    void MysqlConnection::kill_process(std::unordered_map<std::string, std::string> data)
    {
        this->execute("KILL "+ data["Id"]);
    }

    MysqlConnection::~MysqlConnection()
    {
        if(this->_connection)
        {
            this->_connection->close();
            delete this->_connection;
			this->_connection = nullptr;
        }
        if(this->_statement)
        {
            this->_statement->close();
            delete this->_statement;
			this->_statement = nullptr;
        }
    }
};
