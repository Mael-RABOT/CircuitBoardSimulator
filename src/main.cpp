#include "Manager/Manager.hpp"

int main() {
    try {
        /* Init */
        nts::Manager circuit;
        circuit.factory("clock", "i1", nts::Tristate::True);
        //todo: destruct
        circuit.factory("input", "i2", nts::Tristate::True);
        circuit.factory("output", "o1");
        circuit.factory("and", "and1");
        circuit.factory("output", "o2");

        /* Link */
        circuit.getComponent("and1")->setLink(1, *circuit.getInput("i1"), 1);
        circuit.getComponent("and1")->setLink(2, *circuit.getInput("i2"), 1);
        circuit.getOutput("o1")->setLink(1, *circuit.getComponent("and1"), 3);
        circuit.getOutput("o2")->setLink(1, *circuit.getComponent("and1"), 3);

        /* Display */
        circuit.simulate(1);
        circuit.debug();
        circuit.simulate(2);
        circuit.debug(true, false, true);
        circuit.simulate();
        circuit.debug(true, false, true);
        circuit.simulate();
        circuit.debug(true, false, true);
        circuit.simulate();
        circuit.debug();

    } catch (const nts::CustomError &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
