#pragma once
#include <pqxx/pqxx>

class Database {
private:
    std::unique_ptr<pqxx::connection> conn;
    void connectDb(const std::string& db_url);
public:
    explicit Database(const std::string& db_url);
    void saveOperation(const std::string& var);
    std::vector<std::string> fetchVars();
};