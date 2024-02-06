#pragma once

#include <map>
#include <vector>
#include <functional>
#include <utility>

#include "../Component/IComponent.hpp"

namespace nts {
    class Pin {
        private:
            std::size_t _id;
            std::size_t _lastTick;
            nts::Tristate _state;
            std::vector<std::pair<std::reference_wrapper<IComponent>, std::reference_wrapper<Pin>>> _links;
            bool _isInput;

        public:
            Pin();
            Pin(std::size_t id, nts::Tristate state);
            ~Pin();
            nts::Tristate getState() const;
            void setState(nts::Tristate state);
            void setLink(nts::IComponent &component, nts::Pin &other);
            void setIsInput(bool isIsInput);
            Tristate updatePin(std::size_t tick);
    };
}
