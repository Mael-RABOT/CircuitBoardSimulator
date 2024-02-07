#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Clock : public AComponent {
    public:
        Clock() : AComponent(1, nts::ComponentType::INPUT) {
            this->getPin(1).setState(nts::Tristate::True);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            if (tick == 0)
                return this->getPin(1).setState(Tristate::Undefined);
            this->getPin(1).setState((tick % 2 == 0) ? Tristate::False : Tristate::True);
        };

        nts::Tristate compute(std::size_t pin) {
            if (pin != 1)
                throw nts::CustomError("Pin not found: " + std::to_string(pin));
            if (_lastTick == 0)
                return nts::Tristate::Undefined;
            return this->getPin(pin).getState();
        };
    };
}
