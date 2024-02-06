#include <iostream>

#include "Component/AComponent.hpp"

int main() {
    nts::AComponent component(4);
    for (std::size_t i = 1; i <= 4; i++) {
        std::cout << "state: " << component.compute(i) << std::endl;
    }

    for (std::size_t i = 1; i <= 4; i++) {
        component.getPin(i).setState(nts::Tristate::True);
    }

    for (std::size_t i = 1; i <= 4; i++) {
        std::cout << "state: " << component.compute(i) << std::endl;
    }
    return 0;
}
