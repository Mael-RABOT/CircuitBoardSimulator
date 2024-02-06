#include "AComponent.hpp"

namespace nts {
    AComponent::AComponent(std::size_t nbPin) : IComponent() {
        for (std::size_t i = 1; i <= nbPin; i++) {
            _pins[i] = nts::Pin(i, Tristate::Undefined);
        }
        _lastTick = 0;
    }

    void AComponent::simulate(std::size_t tick) {
        if (tick <= _lastTick)
            return;
        _lastTick = tick;
        for (auto &pin : _pins) {
            pin.second.updatePin(tick);
        }
    }

    nts::Tristate AComponent::compute(std::size_t pin) {
        if (_pins.find(pin) == _pins.end()) {
            throw nts::CustomError("Pin not found");
        }
        return _pins[pin].getState();
    }

    void AComponent::setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) {
        _pins[pin].setLink(other, other.getPin(otherPin));
        other.reversedLink(otherPin, *this, pin);
    }

    void AComponent::reversedLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) {
        _pins[pin].setLink(other, other.getPin(otherPin));
    }

    Pin &AComponent::getPin(std::size_t pin) {
        if (_pins.find(pin) == _pins.end()) {
            throw nts::CustomError("Pin not found");
        }
        return _pins[pin];
    }
}
