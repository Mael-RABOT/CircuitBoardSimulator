#pragma once

#include "../AComponent.hpp"

namespace nts {
    class TrueC : public AComponent {
    public:
        TrueC() : AComponent(1, nts::ComponentType::COMPONENT) {
            this->getPin(1).setState(nts::Tristate::True);
        };

        void simulate(std::size_t tick) {
            this->_lastTick = tick;
            this->getPin(1).setState(nts::Tristate::True);
        };
    };
}
