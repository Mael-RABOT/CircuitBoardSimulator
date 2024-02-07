#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Output : public AComponent {
        public:
            Output() : AComponent(1, nts::ComponentType::OUTPUT) {};

            void simulate(std::size_t tick) {
                this->_pins[1].updatePin(tick);
            };
    };
}
