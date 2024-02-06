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
            std::size_t _lastTick;

        public:
            Manager();
            void addComponent(const std::string &label, IComponent *component, ComponentType type);

            void factory(const std::string &type, const std::string &label) {
                this->factory(type, label, nts::Tristate::Undefined);
            };
            void factory(
                const std::string &type,
                const std::string &label,
                nts::Tristate state);

            void addLink(
                    const std::string &label,
                    ComponentType type,
                    std::size_t sourcePin,
                    nts::IComponent &other,
                    std::size_t otherPin);

            void debug() const { this->debug(true, true, true); };
            void debug(bool, bool, bool) const;

            void simulate() { this->simulate(_lastTick + 1); };
            void simulate(std::size_t tick);

            std::map<std::string, IComponent *> getInputs() { return _inputs; };
            std::map<std::string, IComponent *> getOutputs() { return _outputs; };
            std::map<std::string, IComponent *> getComponents() { return _components; };
            IComponent *getInput(const std::string &label) { return _inputs[label]; };
            IComponent *getOutput(const std::string &label) { return _outputs[label]; };
            IComponent *getComponent(const std::string &label) { return _components[label]; };
    };
}
