#include <iostream>

#include "../include/Config.h"
#include "../include/Database.h"
#include "../include/Logger.h"

Logger apiLogger("../logs/api.log");

int main() {
    Config cfg;
    auto env = cfg.getEnv();

    Database db(env.db_url);
    auto vars = db.fetchVars();

    for (const auto& var : vars) {
        std::cout << var.first << '\n';
        std::cout << var.second << '\n';
    }
}
