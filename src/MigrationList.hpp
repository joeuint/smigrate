#pragma once

#include <string>
#include <expected>

#include "MigrationNode.hpp"

class MigrationList {
public:
    MigrationNode* m_head { nullptr }; // last node
    MigrationNode* m_tail { nullptr }; // first node
    
public:
    ~MigrationList();
    void push(const int id, const std::string_view name, const std::string_view upFile, const std::string_view downFile);
    std::expected<void, std::string> loadMigrations(const std::string_view path = "./migrations");
    void print();
};
