#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class C4001 : public AComponent {
    public:
        C4001() : AComponent(14) {};
        void simulate(std::size_t tick);
    };
}
