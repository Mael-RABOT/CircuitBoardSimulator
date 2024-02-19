#include <criterion/criterion.h>

#include "../include/paths.hpp"
#include "../src/Manager/Manager.hpp"

nts::Manager *FourBitAdderManager;

void FourBitAdderSetup(void) {
    FourBitAdderManager = new nts::Manager();
    FourBitAdderManager->initializeTruthTables(TRUTH_TABLE_PATH);
    FourBitAdderManager->initGates(PRIMARY_GATES_PATH);
    FourBitAdderManager->initGates(SECONDARY_GATES_PATH);
    char av1[] = "./a.out";
    char av2[] = "./test/nts_single/4008_adder.nts";
    char *av[] = {av1, av2};
    FourBitAdderManager->parser(2, av);
}

void FourBitAdderTeardown(void) {
    delete FourBitAdderManager;
}

TestSuite(four_bit_adder_tests, .init = FourBitAdderSetup, .fini = FourBitAdderTeardown);

std::vector<std::vector<nts::Tristate>> generateTestCases() {
    std::vector<std::vector<nts::Tristate>> testCases;

    for (int i = 0; i < 512; ++i) {
        std::vector<nts::Tristate> testCase(14, nts::Tristate::False);

        for (int j = 0; j < 9; ++j) {
            if (i & (1 << j)) {
                testCase[j] = nts::Tristate::True;
            }
        }

        int sum = 0;
        for (int j = 0; j < 8; ++j) {
            if (testCase[j] == nts::Tristate::True) {
                sum += (1 << j);
            }
        }
        if (testCase[8] == nts::Tristate::True) {
            sum += 1;
        }

        for (int j = 0; j < 5; ++j) {
            if (sum & (1 << j)) {
                testCase[9+j] = nts::Tristate::True;
            }
        }

        testCases.push_back(testCase);
    }

    return testCases;
}

Test(four_bit_adder_tests, test_four_bit_adder)
{
    std::vector<std::vector<nts::Tristate>> testCases = generateTestCases();

    for (auto& testCase : testCases) {
        std::vector<std::string> inputNames = {"in_a1", "in_a2", "in_a3", "in_a4", "in_b1", "in_b2", "in_b3", "in_b4", "in_c"};
        for (int i = 0; i < 9; ++i) {
            std::string componentName = inputNames[i];
            nts::Tristate value = testCase[i];
            FourBitAdderManager->getComponent(componentName)->setValue(value);
        }

        FourBitAdderManager->simulate();

        std::vector<std::string> outputNames = {"out_0", "out_1", "out_2", "out_3", "out_c"};
        for (unsigned int i = 0; i < outputNames.size(); ++i) {
            std::string componentName = outputNames[i];
            nts::Tristate expectedValue = testCase[9+i];
            nts::Tristate actualValue = FourBitAdderManager->getComponent(componentName)->getValue(1);
            cr_assert_eq(actualValue, expectedValue,
                "Failed on %s with inputs in_a1=%d, in_a2=%d, in_a3=%d, in_a4=%d, in_b1=%d, in_b2=%d, in_b3=%d, in_b4=%d, in_c=%d. Expected: %d, but got: %d",
                componentName.c_str(), testCase[0], testCase[1], testCase[2], testCase[3],
                testCase[4], testCase[5], testCase[6], testCase[7], testCase[8],expectedValue, actualValue);
        }
    }
}
