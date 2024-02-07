#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class Not : public AComponent {
    public:
        Not() : AComponent(2, nts::ComponentType::COMPONENT) {};
        void simulate(std::size_t tick);
    };
}
