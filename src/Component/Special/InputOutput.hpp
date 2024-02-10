#pragma once

#include "../AComponent.hpp"

namespace nts {
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
