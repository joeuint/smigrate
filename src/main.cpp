#include <cctype>
#include <memory>
#include <fstream>
#include <print>
#include <cassert>
#include <string>
#include <regex>
#include <filesystem>
#include <unordered_map>

constexpr std::string_view filenameRegex { "^([0-9]{6})_([a-zA-Z_]+)\\.(up)\\.sql" };

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

class MigrationList {
public:
    MigrationNode* m_head { nullptr }; // last node
    MigrationNode* m_tail { nullptr }; // first node
    
public:
    ~MigrationList();
    void push(const int id, const std::string_view name, const std::string_view upFile, const std::string_view downFile);
    void loadMigrations(const std::string_view path = "./migrations");
    void print();
};

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

MigrationNode::MigrationNode(const int id, const std::string_view name, std::string_view upFile, std::string_view downFile)
    : id(id), name(name), upFile(upFile), downFile(downFile) {};

void MigrationList::loadMigrations(const std::string_view path) {
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
}
// TODO: It's all a mess. break it into seperate files.
class SubCommandHandler {
public:
    virtual void execute(int argc, const std::vector<std::string>& argv, MigrationList& migs) = 0;
    virtual ~SubCommandHandler() = default;
};

class NewHandler : public SubCommandHandler {
    int getNewNodeId(MigrationNode* tail) {
        int newId = 0;
        
        while (tail != nullptr) {
            newId = std::max(newId, tail->getId() + 1);

            tail = tail->next;
        }

        return newId;
    }

    bool verifyCharset(const std::string& name) {
        return std::all_of(
            name.begin(),
            name.end(),
            [](char c) {
                return std::isalpha(c) || c == '_';
            }
        );

    }

    void execute(int argc, const std::vector<std::string>& argv, MigrationList& migs) override {

        if (argc < 3) {
            std::println("Not enough arguments");
            return;
        }

        if (!verifyCharset(argv[2])) {
            std::println("Only letters and underscores are allowed in names!");
            return;
        }
                

        migs.loadMigrations();


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
};

class SubCommandParser {
    const int m_argc;
    std::vector<std::string> m_argv;
    
    MigrationList& m_migs;

    std::unordered_map<std::string, std::unique_ptr<SubCommandHandler>> m_cmds;

    void addCmds() {
        m_cmds.emplace("new", std::make_unique<NewHandler>());
    }

public:

    SubCommandParser(int argc, char* argv[], MigrationList& migs) : m_argc(argc), m_migs(migs) {
        m_argv.reserve(argc);

        for (int i = 0; i < argc; i++) {
            m_argv.push_back(argv[i]);
        }
    }

    void runCmds() {
        addCmds();

        if (m_argc < 2) {
            std::println("Not enough arguments");
            return;
        }

        auto it = m_cmds.find(m_argv[1]);

        if (it == m_cmds.end()) {
            std::println("subcmd not found");
            return;
        }

        it->second->execute(m_argc, m_argv, m_migs);
    };
};


int main(int argc, char* argv[]) {
    MigrationList migs;

    SubCommandParser subCmd { argc, argv, migs };

    subCmd.runCmds();
}
