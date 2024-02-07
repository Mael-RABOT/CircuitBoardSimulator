#include "Pin.hpp"

namespace nts {
    Pin::Pin() : _id(0), _state(nts::Tristate::Undefined), _isInput(false) {
        _lastTick = 0;
    }

    Pin::Pin(std::size_t id, nts::Tristate state) : _id(id), _state(state), _isInput(false) {
        _lastTick = 0;
    }

    Pin::~Pin() {}

    nts::Tristate Pin::getState() const {
        return _state;
    }

    void Pin::setState(nts::Tristate state) {
        _state = state;
    }

    void Pin::setLink(nts::IComponent &component, nts::Pin &other) {
        _links.push_back(std::make_pair(std::ref(component), std::ref(other)));
    }

    void Pin::setIsInput(bool isIsInput) {
        _isInput = isIsInput;
    }

    Tristate Pin::updatePin(std::size_t tick) {
        if (tick <= _lastTick)
            return _state;
        _lastTick = tick;
        if (_isInput) {
            if (tick == 0)
                return nts::Tristate::Undefined;
            return _state;
        }
        _state = Tristate::Undefined;
        if (_links.empty()) {
            return _state;
        }
        for (auto &link : _links) {
            link.first.get().simulate(tick);
            _state = std::max(_state, link.second.get().updatePin(tick));
        }
        return _state;
    }
}
