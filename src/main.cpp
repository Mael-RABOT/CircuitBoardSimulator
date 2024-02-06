#include "Component/AComponent.hpp"
#include "Component/And/And.hpp"
#include "Component/Input/Input.hpp"
#include "Component/Clock/Clock.hpp"

int main() {
    try {
        nts::IComponent *input = new nts::Input(nts::Tristate::True);
        nts::IComponent *inp2 = new nts::Input(nts::Tristate::True);
        nts::IComponent *andt = new nts::And();
        andt->setLink(1, *input, 1);
        andt->setLink(2, *inp2, 1);

        std::cout << "Input 1: " << input->getPin(1).getState() << std::endl;
        std::cout << "Input 2: " << inp2->getPin(1).getState() << std::endl;
        std::cout << "and output: " << andt->getPin(3).getState() << std::endl;

        andt->simulate(1);
        andt->simulate(1);

        std::cout << "and output: " << andt->getPin(3).getState() << std::endl;

    } catch (const nts::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
