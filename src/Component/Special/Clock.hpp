#pragma once

#include "../AComponent.hpp"

namespace nts {
    class Clock : public AComponent {
        public:
            nts::Tristate _newValue;
            bool needUpdate;

            Clock(const std::string &label) : AComponent(1, label, ComponentType::Input) {};
            void computeBehaviour(std::size_t tick) {
                _lastTick = tick;
                if (_pins[1].first == nts::Tristate::Undefined) return;
                if (!needUpdate)
                    return (void)(_pins[1].first = _pins[1].first == nts::Tristate::True ? nts::Tristate::False : nts::Tristate::True);
                this->setState(1, _newValue);
                needUpdate = false;
            }
            void setValue(nts::Tristate value) {
                _newValue = value;
                needUpdate = true;
            }
    };
}
