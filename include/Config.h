#pragma once

#include <string>

typedef struct {
    std::string db_url;
    std::string apikey;
} Env;

class Config {
private:
    Env env_vars;
    void loadEnv();
public:
    Config();
    [[nodiscard]] const Env& getEnv() const;
};