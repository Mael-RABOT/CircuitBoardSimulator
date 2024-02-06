#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Output : public AComponent {
        public:
            Output() : AComponent(1) {};

            void simulate(std::size_t tick) {
                this->_pins[1].updatePin(tick);
            };

            nts::Tristate compute(std::size_t pin) {
                if (pin != 1)
                    throw nts::CustomError("Pin not found");
                return this->getPin(pin).getState();
            };
    };
}
