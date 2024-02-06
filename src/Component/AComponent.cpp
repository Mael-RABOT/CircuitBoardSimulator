#include "AComponent.hpp"

namespace nts {
    AComponent::AComponent(std::size_t nbPin) : IComponent() {
        for (std::size_t i = 1; i <= nbPin; i++) {
            _pins[i] = nts::Pin(i, Tristate::Undefined);
        }
    }

    void AComponent::simulate(std::size_t tick) {
        _tick = tick;
        for (auto &pin : _pins) {
            pin.second.updatePin(_tick);
        }
    }

    nts::Tristate AComponent::compute(std::size_t pin) {
        if (_pins.find(pin) == _pins.end()) {
            throw nts::Error("Pin not found");
        }
        return _pins[pin].updatePin(_tick);
    }

    void AComponent::setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) {
        _pins[pin].setLink(other.getPin(otherPin));
    }

    Pin &AComponent::getPin(std::size_t pin) {
        if (_pins.find(pin) == _pins.end()) {
            throw nts::Error("Pin not found");
        }
        return _pins[pin];
    }
}
