#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class Or : public AComponent {
    public:
        Or() : AComponent(2) {};
        void simulate(std::size_t tick);
    };
}
