#pragma once

#include "../AComponent.hpp"
#include "../../Manager/Manager.hpp"

namespace nts {
    class Manager;
    class GenericGate : public AComponent {
        private:
            std::map<std::string, IComponent *> _components;
            std::map<std::size_t, std::pair<std::string, std::size_t>> _pinRefTable;

        public:
            GenericGate(nts::Manager *manager, const std::string &name, int pinNb,
                        std::vector<std::tuple<std::string, std::string>> componentsData,
                        std::map<std::size_t, std::pair<std::string, std::size_t>> pinRefTable)
                    : AComponent(pinNb, name, ComponentType::Standard), _pinRefTable(pinRefTable) {
                for (auto &data : componentsData) {
                    _components[std::get<1>(data)] = manager->factory(std::get<0>(data), std::get<1>(data));
                }
            }
            ~GenericGate() {
                for (auto &component : _components) {
                    delete component.second;
                }
                _components.clear();
            }

            void updatePins() {
                for (auto &[pin, ref] : _pinRefTable) {
                    auto &[componentName, componentPin] = ref;
                    if (_components.count(componentName) == 0)
                        throw nts::CustomError("Component " + componentName + " not found during updatePins()");

                    auto componentPins = _components[componentName]->getPins();
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
                if (_components.count(componentName) == 0)
                    throw nts::CustomError("Component " + componentName + " not found during setLink()");

                _components[componentName]->setLink(componentPin, other, otherPin);
            }
    };
}
