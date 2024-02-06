#include "AComponent.hpp"

namespace nts {
    AComponent::AComponent(std::size_t nbPin) : IComponent() {
        for (std::size_t i = 1; i <= nbPin; i++) {
            _pins[i] = nts::Pin(Tristate::Undefined);
        }
    }

    void AComponent::simulate(std::size_t) {

    }

    nts::Tristate AComponent::compute(std::size_t pin) {
        return _pins[pin].getState();
    }

    void AComponent::setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) {
        _pins[pin].setLink(otherPin, other.getPin(otherPin));
    }

    Pin &AComponent::getPin(std::size_t pin) {
       return _pins[pin];
    }
}
