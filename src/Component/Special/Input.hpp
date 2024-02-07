#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Input : public AComponent {
        public:
            Input() : AComponent(1, nts::ComponentType::INPUT) {
                this->getPin(1).setState(nts::Tristate::Undefined);
                this->getPin(1).setIsInput(true);
            };

            void simulate(std::size_t) {};
    };
}
