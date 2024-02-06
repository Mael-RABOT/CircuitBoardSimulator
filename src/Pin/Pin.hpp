#pragma once

#include <map>
#include <vector>
#include <functional>

#include "../Component/IComponent.hpp"

namespace nts {
    class Pin {
        private:
            std::size_t _id;
            std::size_t _tick;
            nts::Tristate _state;
            std::vector<std::reference_wrapper<Pin>> _links;

        public:
            Pin();
            Pin(std::size_t id, nts::Tristate state);
            ~Pin();
            nts::Tristate getState() const;
            void setState(nts::Tristate state);
            void setLink(nts::Pin &other);
            Tristate updatePin(std::size_t tick);
    };
}
