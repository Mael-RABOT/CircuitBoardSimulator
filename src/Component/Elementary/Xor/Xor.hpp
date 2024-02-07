#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class Xor : public AComponent {
    public:
        Xor() : AComponent(3) {};
        void simulate(std::size_t tick);
    };
}
