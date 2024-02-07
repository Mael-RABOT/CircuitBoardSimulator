#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Input : public AComponent {
        public:
            Input(Tristate state) : AComponent(1, nts::ComponentType::Input) {
                this->getPin(1).setState(nts::Tristate::Undefined);
                this->getPin(1).setIsInput(true);
            };

            void simulate(std::size_t) {};

            nts::Tristate compute(std::size_t pin) {
                if (pin != 1)
                    throw nts::CustomError("Pin not found: " + std::to_string(pin));
                return this->getPin(pin).getState();
            };
    };
}
