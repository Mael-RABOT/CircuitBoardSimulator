#include "Manager/Manager.hpp"

int main() {
    try {
        nts::Manager circuit;
        circuit.factory("input", "i1", NO_PIN, nts::Tristate::True);
        circuit.factory("input", "i2", NO_PIN, nts::Tristate::True);
        circuit.factory("and", "and1");
        circuit.getComponent("and1")->setLink(1, *circuit.getInput("i1"), 1);
        circuit.getComponent("and1")->setLink(2, *circuit.getInput("i2"), 1);

        std::cout << "Input 1: " << circuit.getInput("i1")->getPin(1).getState() << std::endl;
        std::cout << "Input 2: " << circuit.getInput("i2")->getPin(1).getState() << std::endl;
        std::cout << "and output: " << circuit.getComponent("and1")->getPin(3).getState() << std::endl;

        circuit.getComponent("and1")->simulate(1);

        std::cout << "and output: " << circuit.getComponent("and1")->getPin(3).getState() << std::endl;
    } catch (const nts::CustomError &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
