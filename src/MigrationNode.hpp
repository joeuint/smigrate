#pragma once

#include <string>

class MigrationNode {
    public:
        MigrationNode(const int id, const std::string_view name,
                      const std::string_view upFile,
                      const std::string_view downFile
                      );

        MigrationNode* prev;
        MigrationNode* next;

        int getId() const {
            return id;
        }

        std::string_view getName() const {
            return name;
        }

    private: 
        const int id;
        const std::string name;

        const std::string upFile;
        const std::string downFile;

};
