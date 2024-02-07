#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Clock : public AComponent {
    public:
        Clock() : AComponent(1, nts::ComponentType::INPUT) {
            this->getPin(1).setState(nts::Tristate::Undefined);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState((tick % 2 == 0) ? Tristate::False : Tristate::True);
        };

        nts::Tristate compute(std::size_t pin) {
            if (pin != 1)
                throw nts::CustomError("Pin not found: " + std::to_string(pin));
            return this->getPin(pin).getState();
        };
    };
}
