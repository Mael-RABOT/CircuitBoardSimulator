#include "Manager.hpp"

namespace nts {
    Manager::Manager() : _lastTick(0) {}

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

    void Manager::factory(
            const std::string &type,
            const std::string &label,
            nts::Tristate state = nts::Tristate::Undefined
    ) {
        if (type == "input")
            this->addComponent(label, new nts::Input(state), nts::ComponentType::INPUT);
        else if (type == "clock")
            this->addComponent(label, new nts::Clock(state), nts::ComponentType::INPUT);
        else if (type == "output")
            this->addComponent(label, new nts::Output(), nts::ComponentType::OUTPUT);
        else if (type == "and")
            this->addComponent(label, new nts::And(), nts::ComponentType::COMPONENT);
        else
            throw CustomError("Unknown component: " + type);
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

    void Manager::debug(
            bool inputs __attribute_maybe_unused__ = true,
            bool components __attribute_maybe_unused__ = true,
            bool outputs __attribute_maybe_unused__ = true
    ) const {
        if (inputs) {
            std::cout << "Inputs: " << std::endl;
            for (auto &input: _inputs) {
                std::cout << " - " << input.first << std::endl;
                for (auto &pin: input.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
        if (components) {
            std::cout << std::endl << "Components: " << std::endl;
            for (auto &component: _components) {
                std::cout << " - " << component.first << std::endl;
                for (auto &pin: component.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
        if (outputs) {
            std::cout << std::endl << "Outputs: " << std::endl;
            for (auto &output: _outputs) {
                std::cout << " - " << output.first << std::endl;
                for (auto &pin: output.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
    }

    void Manager::simulate(std::size_t tick) {
        _lastTick = tick;
        for (auto &output : _outputs) {
            output.second->simulate(tick);
        }
    }
}
