#pragma once

#include "../AComponent.hpp"

namespace nts {
    class LogicGate : public AComponent {
        private:
            std::vector<std::vector<nts::Tristate>> _truthTable;

        public:
            LogicGate(std::size_t pinNb, const std::string &label, ComponentType type, std::vector<std::vector<nts::Tristate>> truthTable)
                    : AComponent(pinNb, label, type), _truthTable(truthTable) {};

            void computeBehaviour(std::size_t tick) {
                _lastTick = tick;
                this->update(tick);
                std::vector<nts::Tristate> inputStates;
                for (std::size_t i = 1; i <= _pins.size() - 1; ++i) {
                    for (auto link : _pins[i].second) {
                        inputStates.push_back(link.first.get().getPins()[link.second].first);
                    }
                }

                for (const auto& row : _truthTable) {
                    if (std::equal(inputStates.begin(), inputStates.end(), row.begin())) {
                        _pins[_pins.size()].first = row.back();
                        return;
                    }
                }

                throw nts::CustomError("No match found in truth table");
            };
    };

    class InputComp : public AComponent {
        public:
            InputComp(const std::string &label) : AComponent(1, label, ComponentType::Input) {};
            void computeBehaviour(std::size_t) {};
    };

    class OutputComp : public AComponent {
        public:
            OutputComp(const std::string &label) : AComponent(1, label, ComponentType::Output) {};
            void computeBehaviour(std::size_t tick) { this->update(tick); };
    };
}
