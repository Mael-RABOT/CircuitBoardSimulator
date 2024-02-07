#pragma once

#include "../AComponent.hpp"

namespace nts {
    class CTrue : public AComponent {
    public:
        CTrue() : AComponent(1, nts::ComponentType::INPUT) {
            this->getPin(1).setState(nts::Tristate::True);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState(nts::Tristate::True);
        };
    };
}
