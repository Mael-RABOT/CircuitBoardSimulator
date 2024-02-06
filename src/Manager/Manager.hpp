#pragma once

#include "../../include/Components.hpp"

#define NO_PIN 0

namespace nts {
    enum ComponentType {
        INPUT,
        OUTPUT,
        COMPONENT
    };

    class Manager {
        private:
            std::map<std::string, IComponent *> _inputs;
            std::map<std::string, IComponent *> _outputs;
            std::map<std::string, IComponent *> _components;

        public:
            Manager();
            void addComponent(const std::string &label, IComponent *component, ComponentType type);

            void factory(
                const std::string &type,
                const std::string &label,
                std::size_t nbPins,
                nts::Tristate state);

            void factory(const std::string &type, const std::string &label) {
                this->factory(type, label, NO_PIN, nts::Tristate::Undefined);
            };

            void addLink(
                    const std::string &label,
                    ComponentType type,
                    std::size_t sourcePin,
                    nts::IComponent &other,
                    std::size_t otherPin);

            std::map<std::string, IComponent *> getInputs() { return _inputs; };
            std::map<std::string, IComponent *> getOutputs() { return _outputs; };
            std::map<std::string, IComponent *> getComponents() { return _components; };
            IComponent *getInput(const std::string &label) { return _inputs[label]; };
            IComponent *getOutput(const std::string &label) { return _outputs[label]; };
            IComponent *getComponent(const std::string &label) { return _components[label]; };
    };
}
