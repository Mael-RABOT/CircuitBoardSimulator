#include "Pin.hpp"

namespace nts {
    Pin::Pin() : _id(0), _state(nts::Tristate::Undefined) {}

    Pin::Pin(std::size_t id, nts::Tristate state) : _id(id), _state(state) {}

    Pin::~Pin() {}

    nts::Tristate Pin::getState() const {
        return _state;
    }

    void Pin::setState(nts::Tristate state) {
        _state = state;
    }

    void Pin::setLink(nts::Pin &other) {
        _links.push_back(other);
    }

    Tristate Pin::updatePin(std::size_t tick) {
        if (_tick == tick)
            return _state;
        _tick = tick;
        _state = Tristate::Undefined;
        if (_links.empty()) {
            return _state;
        }
        for (auto &link : _links)
            _state = std::max(_state, link.getState());
        return _state;
    }

    std::vector<Pin> Pin::getLinks() {
        return _links;
    }
}
