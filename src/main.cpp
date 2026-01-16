#include "MigrationList.hpp"
#include "SubCommandParser.hpp"

int main(int argc, char* argv[]) {
    MigrationList migs;

    SubCommandParser subCmd { argc, argv, migs };

    subCmd.runCmds();
}
