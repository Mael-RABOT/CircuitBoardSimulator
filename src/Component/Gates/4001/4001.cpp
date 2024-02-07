#include "4001.hpp"

namespace nts {
    void C4001::simulate(std::size_t tick) {
        for (auto &pin : _pins)
            pin.second.updatePin(tick);
    }
}
