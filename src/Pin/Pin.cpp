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
        _links.push_back(std::ref(other));
    }

    Tristate Pin::updatePin(std::size_t tick) {
        std::cout << "> updatePin " << _id << std::endl;
        if (_tick == tick)
            return _state;
        _tick = tick;
        _state = Tristate::Undefined;
        if (_links.empty()) {
            std::cout << "> No links" << std::endl;
            return _state;
        }
        std::cout << "> State: " << _state << std::endl;
        for (auto &link : _links)
            _state = std::max(_state, link.get().getState());
        return _state;
    }
}
