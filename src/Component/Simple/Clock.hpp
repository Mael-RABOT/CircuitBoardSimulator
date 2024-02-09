#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Clock : public AComponent {
        public:
            Clock(const std::string &label) : AComponent(1, label, ComponentType::Input) {};
            void computeBehaviour(std::size_t tick) {
                _lastTick = tick;
                _pins[1].first = (tick % 2 == 0) ? nts::Tristate::True : nts::Tristate::False;
            }
    };
}
