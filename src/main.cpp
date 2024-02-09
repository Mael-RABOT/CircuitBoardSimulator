#include <iostream>

#include "Manager/Manager.hpp"

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    try {
        nts::Manager manager;
        manager.parser(ac, av);
        manager.run();
    } catch (const nts::CustomError &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
