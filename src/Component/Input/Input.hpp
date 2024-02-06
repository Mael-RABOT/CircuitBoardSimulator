#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Input : public AComponent {
        public:
            Input(Tristate state) : AComponent(1) {
                this->getPin(1).setState(state);
                this->getPin(1).setIsInput(true);
            };

            void simulate(std::size_t) {};

            nts::Tristate compute(std::size_t pin) {
                if (pin != 1)
                    throw nts::CustomError("Pin not found");
                return this->getPin(pin).getState();
            };
    };
}
