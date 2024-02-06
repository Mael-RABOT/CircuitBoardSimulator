#pragma once

#include <unordered_map>
#include <vector>

#include "../Component/IComponent.hpp"

namespace nts {
    class Pin {
        private:
            nts::Tristate _state;
            std::unordered_map<std::size_t, std::vector<Pin>> _links;

        public:
            Pin();
            Pin(nts::Tristate state);
            ~Pin();
            nts::Tristate getState() const;
            void setState(nts::Tristate state);
            void setLink(std::size_t pin, nts::Pin &other);
            std::vector<Pin> getLinks(std::size_t pin);
    };
}
