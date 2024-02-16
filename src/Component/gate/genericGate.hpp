#pragma once

#include "../AComponent.hpp"

namespace nts {
    class GenericGate : public AComponent {
        private:
            std::vector<std::pair<std::string, IComponent *>> _components;
            std::map<std::size_t, std::pair<std::string, std::size_t>> _pinRefTable;

        public:
            GenericGate(const std::string &name, int pinNb,
                        std::vector<
                            std::tuple<
                                std::string, // Internal component name
                                ComponentType, // Internal component type
                                std::vector<std::vector<nts::Tristate>>, // Internal component truth table
                                std::vector<std::size_t>, // Internal component input pins
                                std::vector<std::size_t> // Internal component output pins
                        >> componentsData, // Vector of tuples containing the internal components data
                        std::map<
                            std::size_t, // Internal pin number
                            std::pair<
                                std::string, // Internal component name
                                std::size_t // Internal component pin number
                        >> pinRefTable) // Map of internal pin references
            : AComponent(pinNb, name, ComponentType::Standard), _pinRefTable(pinRefTable) {
                    for (auto &data : componentsData) { // Create the internal components
                        _components.push_back(std::make_pair(std::get<0>(data),
                            new Chipset(3, std::get<0>(data), std::get<1>(data), std::get<2>(data), std::get<3>(data), std::get<4>(data))));
                    }
            };
            ~GenericGate() {
                for (auto &component : _components) {
                    delete component.second;
                }
                _components.clear();
            }

            void updatePins() {
                for (auto &[pin, ref] : _pinRefTable) {
                    auto &[componentName, componentPin] = ref;
                    auto component = std::find_if(
                            _components.begin(),
                            _components.end(),
                            [&](const auto &component) {
                                return component.first == componentName;
                            }
                    );
                    if (component == _components.end())
                        throw nts::CustomError("Component not found during updatePins()");

                    auto componentPins = component->second->getPins();
                    if (componentPins.count(componentPin) > 0) {
                        _pins[pin].first = componentPins[componentPin].first;
                    }
                }
            }
            void computeBehaviour(std::size_t tick) override {
                if (tick <= _lastTick)
                    return;
                _lastTick = tick;
                this->update(tick);
                for (auto &component : _components) {
                    component.second->computeBehaviour(tick);
                }
                this->updatePins();
            }
            void setLink(std::size_t pin, nts::IComponent &other, std::size_t otherPin) override {
                auto &[componentName, componentPin] = _pinRefTable[pin];
                auto component = std::find_if(
                        _components.begin(),
                        _components.end(),
                        [&](const auto &component) { return component.first == componentName; });
                if (component == _components.end())
                    throw nts::CustomError("Component not found during setLink()");

                component->second->setLink(componentPin, other, otherPin);
            }
    };
}
