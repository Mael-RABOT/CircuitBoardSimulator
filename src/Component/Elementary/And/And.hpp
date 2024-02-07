#pragma once

#include "../../AComponent.hpp"

namespace nts {
    class And : public AComponent {
        public:
            And() : AComponent(3, nts::ComponentType::COMPONENT) {};
            void simulate(std::size_t tick);
    };
}
