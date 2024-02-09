#pragma once

#include <vector>

namespace nts {
    const std::vector<std::vector<nts::Tristate>> AND_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::True},
            {Tristate::True, Tristate::False, Tristate::False},
            {Tristate::True, Tristate::Undefined, Tristate::Undefined},
            {Tristate::False, Tristate::True, Tristate::False},
            {Tristate::False, Tristate::False, Tristate::False},
            {Tristate::False, Tristate::Undefined, Tristate::False},
            {Tristate::Undefined, Tristate::True, Tristate::Undefined},
            {Tristate::Undefined, Tristate::False, Tristate::False},
            {Tristate::Undefined, Tristate::Undefined, Tristate::Undefined}
    };

    const std::vector<std::vector<nts::Tristate>> OR_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::True},
            {Tristate::True, Tristate::False, Tristate::True},
            {Tristate::True, Tristate::Undefined, Tristate::True},
            {Tristate::False, Tristate::True, Tristate::True},
            {Tristate::False, Tristate::False, Tristate::False},
            {Tristate::False, Tristate::Undefined, Tristate::Undefined},
            {Tristate::Undefined, Tristate::True, Tristate::True},
            {Tristate::Undefined, Tristate::False, Tristate::Undefined},
            {Tristate::Undefined, Tristate::Undefined, Tristate::Undefined}
    };

    const std::vector<std::vector<nts::Tristate>> XOR_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::False},
            {Tristate::True, Tristate::False, Tristate::True},
            {Tristate::True, Tristate::Undefined, Tristate::Undefined},
            {Tristate::False, Tristate::True, Tristate::True},
            {Tristate::False, Tristate::False, Tristate::False},
            {Tristate::False, Tristate::Undefined, Tristate::Undefined},
            {Tristate::Undefined, Tristate::True, Tristate::Undefined},
            {Tristate::Undefined, Tristate::False, Tristate::Undefined},
            {Tristate::Undefined, Tristate::Undefined, Tristate::Undefined}
    };

    const std::vector<std::vector<nts::Tristate>> NOT_TRUTH_TABLE = {
            {Tristate::True, Tristate::False},
            {Tristate::False, Tristate::True},
            {Tristate::Undefined, Tristate::Undefined}
    };

    const std::vector<std::vector<nts::Tristate>> NAND_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::False},
            {Tristate::True, Tristate::False, Tristate::True},
            {Tristate::True, Tristate::Undefined, Tristate::True},
            {Tristate::False, Tristate::True, Tristate::True},
            {Tristate::False, Tristate::False, Tristate::True},
            {Tristate::False, Tristate::Undefined, Tristate::True},
            {Tristate::Undefined, Tristate::True, Tristate::True},
            {Tristate::Undefined, Tristate::False, Tristate::True},
            {Tristate::Undefined, Tristate::Undefined, Tristate::True}
    };

    const std::vector<std::vector<nts::Tristate>> NOR_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::False},
            {Tristate::True, Tristate::False, Tristate::False},
            {Tristate::True, Tristate::Undefined, Tristate::False},
            {Tristate::False, Tristate::True, Tristate::False},
            {Tristate::False, Tristate::False, Tristate::True},
            {Tristate::False, Tristate::Undefined, Tristate::Undefined},
            {Tristate::Undefined, Tristate::True, Tristate::False},
            {Tristate::Undefined, Tristate::False, Tristate::Undefined},
            {Tristate::Undefined, Tristate::Undefined, Tristate::Undefined}
    };

    const std::vector<std::vector<nts::Tristate>> XNOR_TRUTH_TABLE = {
            {Tristate::True, Tristate::True, Tristate::True},
            {Tristate::True, Tristate::False, Tristate::False},
            {Tristate::True, Tristate::Undefined, Tristate::Undefined},
            {Tristate::False, Tristate::True, Tristate::False},
            {Tristate::False, Tristate::False, Tristate::True},
            {Tristate::False, Tristate::Undefined, Tristate::Undefined},
            {Tristate::Undefined, Tristate::True, Tristate::Undefined},
            {Tristate::Undefined, Tristate::False, Tristate::Undefined},
            {Tristate::Undefined, Tristate::Undefined, Tristate::Undefined}
    };
}