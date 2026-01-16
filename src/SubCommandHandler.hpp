#pragma once

#include <vector>
#include <string>

#include "MigrationList.hpp"

class SubCommandHandler {
public:
    virtual void execute(const std::vector<std::string>& argv, MigrationList& migs) = 0;
    virtual ~SubCommandHandler() = default;
};
