#pragma once

#include "../Pin/Pin.hpp"
#include "../../include/Error/Error.hpp"

namespace nts {
    class AComponent : public IComponent {
        protected:
            std::map<std::size_t, nts::Pin> _pins;
            std::size_t _lastTick;

        public:
            AComponent(std::size_t nbPins);
            void simulate(std::size_t tick);
            nts::Tristate compute(std::size_t pin);
            void setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin);
            Pin &getPin(std::size_t pin);
            std::map<std::size_t, nts::Pin> getPins() { return _pins; };
    };
}
