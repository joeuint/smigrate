#pragma once

#include <string>

#include "../MigrationNode.hpp"
#include "../SubCommandHandler.hpp"

class NewHandler : public SubCommandHandler {
    int getNewNodeId(MigrationNode* tail);

    bool verifyCharset(const std::string& name);

    void execute(const std::vector<std::string>& argv, MigrationList& migs) override;
};


