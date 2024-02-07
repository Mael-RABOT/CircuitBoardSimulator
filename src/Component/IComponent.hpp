#pragma once

#include <memory>
#include <iostream>

#include "../Pin/Pin.hpp"

namespace nts {
    enum Tristate {
        Undefined = (-true),
        True = true,
        False = false
    };

    enum ComponentType {
        INPUT,
        OUTPUT,
        COMPONENT
    };

    class Pin;
    class IComponent {
        public :
            virtual ~IComponent() = default;
            virtual void simulate(std::size_t tick) = 0;
            virtual nts::Tristate compute(std::size_t pin) = 0;
            virtual void setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) = 0;
            virtual void reversedLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) = 0;
            virtual Pin &getPin(std::size_t pin) = 0;
            virtual std::map<std::size_t, nts::Pin> getPins() = 0;
            virtual nts::ComponentType getType() const = 0;
    };
}
