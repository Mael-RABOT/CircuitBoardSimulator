#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Input : public AComponent {
        public:
            Input(Tristate state) : AComponent(1) {
                this->getPin(1).setState(state);
            };

            void simulate(std::size_t tick) {
                this->_tick = tick;
                if (this->getPin(1).getState() == Tristate::Undefined)
                    return;
                this->getPin(1)
                    .setState(this->getPin(1).getState() == Tristate::True ? Tristate::False : Tristate::True);
            };

            nts::Tristate compute(std::size_t pin) {
                if (pin != 1)
                    throw nts::Error("Pin not found");
                return this->getPin(pin).getState();
            };
    };
}
