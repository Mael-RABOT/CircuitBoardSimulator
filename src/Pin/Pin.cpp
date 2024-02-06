#include "Pin.hpp"

namespace nts {
    Pin::Pin() : _state(nts::Tristate::Undefined) {}

    Pin::Pin(nts::Tristate state) : _state(state) {}

    Pin::~Pin() {}

    nts::Tristate Pin::getState() const {
        return _state;
    }

    void Pin::setState(nts::Tristate state) {
        _state = state;
    }

    void Pin::setLink(std::size_t pin, nts::Pin &other) {
        _links[pin].push_back(other);
    }

    std::vector<Pin> Pin::getLinks(std::size_t pin) {
        return _links[pin];
    }
}
