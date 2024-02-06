#pragma once

#include "../Pin/Pin.hpp"

namespace nts {
    class AComponent : public IComponent {
        private:
            std::unordered_map<std::size_t, nts::Pin> _pins;

        public:
            AComponent(std::size_t nbPins);
            void simulate(std::size_t tick);
            nts::Tristate compute(std::size_t pin);
            void setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin);
            Pin &getPin(std::size_t pin);
    };
}
