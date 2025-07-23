#pragma once
#include <pqxx/pqxx>

class Database {
private:
    std::unique_ptr<pqxx::connection> conn;
    void connectDb(const std::string& db_url);
public:
    explicit Database(const std::string& db_url);
    void saveOperation(const std::pair<std::string, std::string>& keyValue);
    void updateOperation(const std::pair<std::string, std::string>& keyValue);
    void deleteOperation(const std::string& key);
    [[nodiscard]] std::unordered_map<std::string, std::string> fetchVars() const;
};