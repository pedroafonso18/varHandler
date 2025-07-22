#include "../include/Database.h"
#include <iostream>

void Database::connectDb(const std::string& db_url) {
    try {
        conn = std::make_unique<pqxx::connection>(db_url);
        if (!conn->is_open()) {
            std::cout << "Falha ao abrir conexão com o banco de dados\n";
            conn = nullptr;
        }
    } catch (const std::exception& e) {
        std::cout << "Erro na conexão com o banco de dados: " << std::string(e.what()) << '\n';
        conn = nullptr;
    }
}


Database::Database(const std::string& db_url) : conn(nullptr) {
    connectDb(db_url);
}

std::vector<std::string> Database::fetchVars() {
    try {
        if (!conn || !conn->is_open()) {
            std::cout << ""
        }
    }
}

