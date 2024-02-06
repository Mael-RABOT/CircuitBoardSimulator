#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Clock : public AComponent {
    public:
        Clock(Tristate state) : AComponent(1) {
            this->getPin(1).setState(state);
            this->getPin(1).setIsInput(true);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState((tick % 2 == 0) ? Tristate::False : Tristate::True);
        };

        nts::Tristate compute(std::size_t pin) {
            if (pin != 1)
                throw nts::CustomError("Pin not found");
            return this->getPin(pin).getState();
        };
    };
}
