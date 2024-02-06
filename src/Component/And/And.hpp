#pragma once

#include "../AComponent.hpp"

namespace nts {
    class And : public AComponent {
        public:
            And() : AComponent(3) {};
            void simulate(std::size_t tick);
    };
}
