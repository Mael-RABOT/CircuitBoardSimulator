#include "Manager/Manager.hpp"

int main(int ac, char **av) {
    try {
        nts::Manager manager;
        manager.initializeTruthTables("./Config/TruthTable/");
        manager.preParse(ac, av);
        manager.initGates("./Config/Gates/");
        manager.parser(ac, av);
        manager.run();
    } catch (const nts::CustomError &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
