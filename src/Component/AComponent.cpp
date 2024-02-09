#include "AComponent.hpp"

namespace nts {
    AComponent::AComponent(int pinNb, const std::string &label, ComponentType type)
    : _lastTick(0), _label(label), _type(type) {
        for (int i = 1; i <= pinNb; ++i) {
            _pins[i] = {nts::Tristate::Undefined, {}};
        }
    }

    void AComponent::simulate(std::size_t tick) {
        for (std::size_t i = 0; i < tick; ++i)
            this->update(i);
    }

    nts::Tristate AComponent::_internalCompute(std::size_t pin) {
        std::stack<nts::Tristate> linkedStates;
        for (auto link : _pins[pin].second) {
            nts::Tristate linkedPinState = link.first.get().getPins()[link.second].first;
            linkedStates.push(linkedPinState);
        }

        if (linkedStates.empty())
            return _pins[pin].first;
        if (linkedStates.size() == 1)
            return linkedStates.top();

        while (linkedStates.size() > 1) {
            nts::Tristate state1 = linkedStates.top(); linkedStates.pop();
            nts::Tristate state2 = linkedStates.top(); linkedStates.pop();
            for (const auto& row : nts::OR_TRUTH_TABLE) {
                if ((state1 == row[0] && state2 == row[1]) || (state1 == row[1] && state2 == row[0])) {
                    linkedStates.push(row[2]);
                    break;
                }
            }
        }

        return linkedStates.top();
    }

    nts::Tristate AComponent::compute(std::size_t pin) {
        _pins[pin].first = this->_internalCompute(pin);
        return _pins[pin].first;
    }

    void AComponent::update(std::size_t tick) {
        if (tick <= _lastTick)
            return;
        _lastTick = tick;
        for (auto &[pin, data] : _pins) {
            for (auto &[component, componentPin] : data.second) {
                component.get().computeBehaviour(tick);
            }
            this->compute(pin);
        }
    }

    void AComponent::setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) {
        _pins[pin].second.push_back(std::make_pair(std::ref(other), otherPin));
    }

    void AComponent::removeLink(std::size_t pin) {
        _pins[pin].second.clear();
    }

    void AComponent::dump() const {
        std::cout << _label << " has " << _pins.size() << " pins" << std::endl;
        for (const auto &[pin, _] : _pins) {
            std::cout << "    - Pin " << pin << " has state: "
                << (_pins.at(pin).first == nts::Tristate::True ? "True" :
                    _pins.at(pin).first == nts::Tristate::False ? "False" : "Undefined")
                << std::endl;
            std::cout << "     - Pin " << pin << " is linked to: ";
            for (const auto &[component, componentPin] : _pins.at(pin).second) {
                std::cout << component.get().getLabel() << ":" << componentPin << " ";
            }
            std::cout << std::endl;
        }
    }
}
