#include "Manager.hpp"

namespace nts {
    Manager::Manager() {}

    void Manager::addComponent(const std::string &label, IComponent *component, ComponentType type) {
        switch (type) {
            case ComponentType::INPUT:
                _inputs[label] = component;
                break;
            case ComponentType::OUTPUT:
                _outputs[label] = component;
                break;
            case ComponentType::COMPONENT:
                _components[label] = component;
                break;
        }
    }

    void Manager::addLink(
            const std::string &label,
            nts::ComponentType type,
            std::size_t sourcePin,
            nts::IComponent &other,
            std::size_t otherPin
    ) {
        switch (type) {
            case ComponentType::INPUT:
                _inputs[label]->setLink(sourcePin, other, otherPin);
                break;
            case ComponentType::OUTPUT:
                _outputs[label]->setLink(sourcePin, other, otherPin);
                break;
            case ComponentType::COMPONENT:
                _components[label]->setLink(sourcePin, other, otherPin);
                break;
        }
    }

    void Manager::factory(
            const std::string &type,
            const std::string &label,
            std::size_t nbPins __attribute_maybe_unused__ = NO_PIN,
            nts::Tristate state = nts::Tristate::Undefined
    ) {
        if (type == "input")
            this->addComponent(label, new nts::Input(state), nts::ComponentType::INPUT);
        else if (type == "output")
            this->addComponent(label, new nts::Output(), nts::ComponentType::OUTPUT);
        else if (type == "clock")
            this->addComponent(label, new nts::Clock(state), nts::ComponentType::INPUT);
        else if (type == "and")
            this->addComponent(label, new nts::And(), nts::ComponentType::COMPONENT);
        else
            throw CustomError("Unknown component: " + type);
    }
}
