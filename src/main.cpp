#include "Manager/Manager.hpp"

int main(int ac, char **av) {
    try {
        nts::Manager manager;
        manager.parser(ac, av);

        /* Tests */
        manager.debug();

        std::cout << "------" << std::endl;
        manager.getComponent("in_1")->getPin(1).setState(nts::Tristate::True);
        manager.getComponent("in_2")->getPin(1).setState(nts::Tristate::True);
        manager.simulate();
        manager.debug();

        std::cout << "------" << std::endl;
        manager.getComponent("in_1")->getPin(1).setState(nts::Tristate::False);
        manager.getComponent("in_2")->getPin(1).setState(nts::Tristate::True);
        manager.simulate();
        manager.debug();

    } catch (const nts::CustomError &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
