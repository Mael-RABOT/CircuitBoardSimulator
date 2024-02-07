#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class Not : public AComponent {
    public:
        Not() : AComponent(2) {};
        void simulate(std::size_t tick);
    };
}
