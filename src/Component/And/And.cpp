#include "And.hpp"

namespace nts {
    void And::simulate(std::size_t tick) {
        for (auto &pin : _pins) {
            pin.second.updatePin(tick);
        }
        if (_pins[1].getState() == Tristate::Undefined || _pins[2].getState() == Tristate::Undefined) {
            _pins[3].setState(Tristate::Undefined);
            return;
        }
        if (_pins[1].getState() == Tristate::True && _pins[2].getState() == Tristate::True) {
            _pins[3].setState(Tristate::True);
        } else {
            _pins[3].setState(Tristate::False);
        }
    }
}
