#include "And.hpp"

namespace nts {
    void And::simulate(std::size_t tick) {
        for (auto &pin : _pins) {
            pin.second.updatePin(_tick);
        }
        _tick = tick;
        if (_pins[1].getState() == Tristate::Undefined || _pins[2].getState() == Tristate::Undefined) {
            std::cout << "  - And gate has an undefined state" << std::endl;
            _pins[3].setState(Tristate::Undefined);
            return;
        }
        if (_pins[1].getState() == Tristate::True && _pins[2].getState() == Tristate::True) {
            std::cout << "  - And gate has a true state" << std::endl;
            std::cout << "  - pin3 bef: " << _pins[3].getState() << std::endl;
            _pins[3].setState(Tristate::True);
            std::cout << "  - pin3 aft: " << _pins[3].getState() << std::endl;
        } else {
            _pins[3].setState(Tristate::False);
        }
    }
}
