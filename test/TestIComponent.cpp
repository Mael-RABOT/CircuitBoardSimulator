#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../include/paths.hpp"
#include "../src/Manager/Manager.hpp"

nts::Manager IComponentManager;

void IComponentSetup() {
    IComponentManager.initializeTruthTables(TRUTH_TABLE_PATH);
}

void IComponentTeardown() {
    IComponentManager.clearComponents();
}

TestSuite(IComponent, .init = IComponentSetup, .fini = IComponentTeardown);

Test(IComponent, should_create_component)
{
    IComponentManager.createComponent("input", "in1");
    nts::IComponent *component = IComponentManager.getComponent("in1");

    cr_assert_eq(component->getLabel(), "in1");
    cr_assert_eq(component->getType(), nts::ComponentType::Input);
}

Test(IComponent, should_link_input_and_output)
{
    IComponentManager.createComponent("input", "in1");
    IComponentManager.createComponent("output", "out1");
    IComponentManager.addLink("in1", 1, "out1", 1);

    nts::IComponent *output = IComponentManager.getComponent("out1");
    cr_assert_eq(output->getValue(1), nts::Tristate::Undefined);

    IComponentManager.simulate(0);
    cr_assert_eq(output->getValue(1), nts::Tristate::Undefined);

    IComponentManager.getComponent("in1")->setValue(nts::Tristate::True);
    IComponentManager.simulate(1);
    cr_assert_eq(output->getValue(1), nts::Tristate::True);
}
