#pragma once

#include "../AComponent.hpp"

namespace nts {
    class CFalse : public AComponent {
    public:
        CFalse() : AComponent(1, nts::ComponentType::INPUT) {
            this->getPin(1).setState(nts::Tristate::False);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState(nts::Tristate::False);
        };
    };
}
