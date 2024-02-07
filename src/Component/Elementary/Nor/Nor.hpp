#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class Nor : public AComponent {
    public:
        Nor() : AComponent(3, nts::ComponentType::COMPONENT) {};
        void simulate(std::size_t tick);
    };
}
