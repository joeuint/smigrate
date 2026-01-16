#include <print>
#include <cassert>
#include <regex>
#include <filesystem>

#include "MigrationList.hpp"

constexpr std::string_view filenameRegex { "^([0-9]{6})_([a-zA-Z_]+)\\.(up)\\.sql" };

void MigrationList::push(const int id, const std::string_view name, const std::string_view upFile, const std::string_view downFile) {
    MigrationNode* node { new MigrationNode(id, name, upFile, downFile) };

    node->next = nullptr;
    
    if (m_head && m_tail) { // two or more
        node->prev = m_head;
        m_head->next = node;
        m_head = node;
    } else { // nothing
        m_tail = node;
        m_head = node;
        node->prev = nullptr;
    }
}

void MigrationList::print() {
    auto node { m_tail };

    while (node != nullptr) {
        std::println("{} - {}", node->getId(), node->getName());

        node = node->next;
    }
}

MigrationList::~MigrationList() {
    MigrationNode* node { m_tail };

    MigrationNode* temp;

    while (node != nullptr) {
        temp = node->next;
        delete node;
        node = temp;
    }
}

[[nodiscard]] std::expected<void, std::string> MigrationList::loadMigrations(const std::string_view path) {
    if (!std::filesystem::is_directory(path)) {
        return std::unexpected("No ./migrations directory found.");
    }

    for (const auto& ent : std::filesystem::directory_iterator(path)) {
        if (!ent.is_regular_file() && !ent.path().has_filename()) {
            continue;
        }

        std::regex r(std::string { filenameRegex });

        std::smatch matches;

        // we require a binding here as temp strings are not allowed
        auto filename = ent.path().filename().string();

        std::regex_search(filename, matches, r);

        if (matches.length() == 0) {
            continue;
        }
        
        assert(matches.size() == 4 && "must be extactly 4 matches if found");

        // find the down file
        const std::string downFilePath = "./migrations/" + std::format("{:06}_{}.down.sql", std::stoi(matches[1].str()), matches[2].str());

        if (!std::filesystem::exists(downFilePath)) {
            std::println("No down file detected for migration, skipping...");
            continue;
        }

        push(std::stoi(matches[1].str()), matches[2].str(), path, downFilePath);
    }

    return {};
}
