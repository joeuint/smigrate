#include <algorithm>
#include <fstream>
#include <print>

#include "NewHandler.hpp"

int NewHandler::getNewNodeId(MigrationNode* tail) {
    int newId = 0;
    
    while (tail != nullptr) {
        newId = std::max(newId, tail->getId() + 1);

        tail = tail->next;
    }

    return newId;
}

// TOOD: get a string view?
bool NewHandler::verifyCharset(const std::string& name) {
    return std::all_of(
        name.begin(),
        name.end(),
        [](char c) {
            return std::isalpha(c) || c == '_';
        }
    );

}


void NewHandler::execute(const std::vector<std::string>& argv, MigrationList& migs) {

    if (argv.size() < 3) {
        std::println("Not enough arguments");
        return;
    }

    if (!verifyCharset(argv[2])) {
        std::println("Only letters and underscores are allowed in names!");
        return;
    }
            
    auto migrationResult { migs.loadMigrations() };

    if (!migrationResult) {
        std::println("{}", migrationResult.error());
        return;
    };

    int newId = getNewNodeId(migs.m_tail);

    std::println("{}", newId);

    std::string format = std::format("./migrations/{:06}_{}", newId, argv[2]);

    std::string upFile = format + ".up.sql";
    std::string downFile = format + ".down.sql";

    // TODO: Validate via the regex just to be safe

    std::println("Created: {}", upFile);
    std::println("Created: {}", downFile);
    
    (void)std::ofstream { upFile };
    (void)std::ofstream { downFile };
}
