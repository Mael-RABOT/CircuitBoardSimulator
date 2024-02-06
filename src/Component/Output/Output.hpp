#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Output : public AComponent {
        public:
            Output() : AComponent(1) {};

            void simulate(std::size_t tick) {
                this->_tick = tick;
                this->_pins[1].updatePin(this->_tick);
                std::cout << this->_pins[1].getState() << std::endl;
            };

            nts::Tristate compute(std::size_t pin) {
                if (pin != 1)
                    throw nts::CustomError("Pin not found");
                return this->getPin(pin).getState();
            };
    };
}
