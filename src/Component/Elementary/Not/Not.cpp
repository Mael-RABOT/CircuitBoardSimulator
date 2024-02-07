#include "Not.hpp"

namespace nts {
    void Not::simulate(std::size_t tick) {
        for (auto &pin : _pins)
            pin.second.updatePin(tick);

        if (_pins[1].getState() == Tristate::Undefined) {
            _pins[2].setState(Tristate::Undefined);
            return;
        }
        if (_pins[1].getState() == Tristate::True)
            _pins[2].setState(Tristate::False);
        else
            _pins[2].setState(Tristate::True);
    }
}
