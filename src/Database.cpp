#include "../include/Database.h"
#include <iostream>
#include <unordered_map>
#include "../include/Logger.h"

extern Logger apiLogger;

void Database::connectDb(const std::string& db_url) {
    apiLogger.debug("Tentando conectar no banco de dados");
    try {
        conn = std::make_unique<pqxx::connection>(db_url);
        if (!conn->is_open()) {
            apiLogger.error("Falha ao abrir conexão com o banco de dados");
            conn = nullptr;
        }
        apiLogger.info("Conexão com banco de dados estabelecida com sucesso");
    } catch (const std::exception& e) {
        apiLogger.error("Erro na conexão com banco de dados: " + std::string(e.what()));
        conn = nullptr;
    }
}


Database::Database(const std::string& db_url) : conn(nullptr) {
    connectDb(db_url);
}

std::unordered_map<std::string, std::string> Database::fetchVars() const {
    std::unordered_map<std::string, std::string> vars;
    try {
        if (!conn || !conn->is_open()) {
            apiLogger.error("Conexão com banco de dados não está aberta");
            return vars;
        }
        pqxx::work wrk(*conn);
        pqxx::result res = wrk.exec(
            "SELECT key, value FROM vars");
        wrk.commit();
        if (res.empty()) {
            apiLogger.info("Nenhum valor encontrado.");
            return vars;
        }
        for (pqxx::row row : res) {
            vars.emplace(row[0].as<std::string>(), row[1].as<std::string>());
        }
        return vars;
    } catch (const std::exception& e) {
        apiLogger.error("Erro ao buscar instância: "+ std::string(e.what()));
        return vars;
    }
}

void Database::saveOperation(const std::pair<std::string, std::string>& keyValue) {
    try {
        if (!conn || !conn->is_open()) {
            apiLogger.error("Conexão com banco de dados não está aberta");
            return;
        }
        pqxx::work wrk(*conn);
        pqxx::result res = wrk.exec(
            "INSERT INTO vars (key, value) VALUES (" + wrk.quote(keyValue.first) + "," + wrk.quote(keyValue.second) + ") RETURNING id");
        wrk.commit();
        if (res.empty()) {
            apiLogger.error("Falha ao inserir dados no banco de dados");
        }
        apiLogger.info("Dado inserido com sucesso!");
    } catch (const std::exception& e) {
        apiLogger.error("Erro ao inserir dados: "+ std::string(e.what()));
        return;
    }
}
