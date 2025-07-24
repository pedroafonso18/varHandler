#include "../include/Config.h"
#include <filesystem>
#include "../include/Logger.h"
#include "../lib/dotenv.h"

extern Logger apiLogger;

Config::Config() {
    std::string root_path = "../.env";
    if (std::filesystem::exists(".env")) {
        root_path = ".env";
    }

    apiLogger.debug("Tentando carregar .env de: " + root_path + '\n');
    dotenv::init(root_path.c_str());
    loadEnv();
}

void Config::loadEnv() {
    env_vars.db_url = dotenv::getenv("DB_URL", "");
    env_vars.apikey = dotenv::getenv("APIKEY", "ABCD1234");
}

const Env &Config::getEnv() const {
    return env_vars;
}

