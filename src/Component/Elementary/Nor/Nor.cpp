#include "Nor.hpp"

namespace nts {
    void Nor::simulate(std::size_t tick) {
        for (auto &pin : _pins)
            pin.second.updatePin(tick);

        if (_pins[1].getState() == Tristate::True || _pins[2].getState() == Tristate::True) {
            _pins[3].setState(Tristate::False);
            return;
        } else if (_pins[1].getState() == Tristate::Undefined || _pins[2].getState() == Tristate::Undefined) {
            _pins[3].setState(Tristate::Undefined);
            return;
        } else {
            _pins[3].setState(Tristate::True);
        }
    }
}
