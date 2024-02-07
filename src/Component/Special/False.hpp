#pragma once

#include "../AComponent.hpp"

namespace nts {
    class FalseC : public AComponent {
    public:
        FalseC() : AComponent(1, nts::ComponentType::COMPONENT) {
            this->getPin(1).setState(nts::Tristate::False);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState(nts::Tristate::False);
        };
    };
}
