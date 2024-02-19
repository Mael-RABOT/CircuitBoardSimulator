#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../include/paths.hpp"
#include "../src/Manager/Manager.hpp"

Test(AComponent, should_create_and_test_component)
{
    nts::Manager manager;

    manager.initializeTruthTables(TRUTH_TABLE_PATH);
    manager.initGates(PRIMARY_GATES_PATH);
    manager.initGates(SECONDARY_GATES_PATH);

    manager.createComponent("input", "in1");
//    auto component = manager.getComponent("in1");
//
//    cr_assert_eq(component.getLabel(), "in1");
//    cr_assert_eq(component.getType(), nts::ComponentType::Input);
}
