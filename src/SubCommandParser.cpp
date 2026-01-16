#include <print>

#include "SubCommandParser.hpp"

#include "subcmds/NewHandler.hpp"

void SubCommandParser::addCmds() {
    m_cmds.emplace("new", std::make_unique<NewHandler>());
}

SubCommandParser::SubCommandParser(int argc, char* argv[], MigrationList& migs) : m_migs(migs) {
    m_argv.reserve(argc);

    for (int i = 0; i < argc; i++) {
        m_argv.push_back(argv[i]);
    }
}


void SubCommandParser::runCmds() {
    addCmds();

    if (m_argv.size() < 2) {
        std::println("Not enough arguments");
        return;
    }

    auto it = m_cmds.find(m_argv[1]);

    if (it == m_cmds.end()) {
        std::println("subcmd not found");
        return;
    }

    it->second->execute(m_argv, m_migs);
};
