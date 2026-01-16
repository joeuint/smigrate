#include "MigrationNode.hpp"

MigrationNode::MigrationNode(const int id, const std::string_view name, std::string_view upFile, std::string_view downFile)
    : id(id), name(name), upFile(upFile), downFile(downFile) {};
