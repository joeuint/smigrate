#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "MigrationList.hpp"
#include "SubCommandHandler.hpp"

class SubCommandParser {
    std::vector<std::string> m_argv;
    
    MigrationList& m_migs;

    std::unordered_map<std::string, std::unique_ptr<SubCommandHandler>> m_cmds;

    void addCmds();

public:

    SubCommandParser(int argc, char* argv[], MigrationList& migs);
    void runCmds();
};
