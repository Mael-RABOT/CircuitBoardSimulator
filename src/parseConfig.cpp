#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>

#include "../include/Components.hpp"

#include "Manager/Manager.hpp"

namespace nts {
    void Manager::_parseGateConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        std::string line;
        std::getline(file, line);
        std::string gateName = line;

        std::getline(file, line);
        int pinNb = std::stoi(line.substr(6)); // Skip "PinNb "

        std::vector<std::tuple<std::string, ComponentType, std::vector<std::vector<nts::Tristate>>, std::vector<std::size_t>, std::vector<std::size_t>>> componentsData;
        std::map<std::size_t, std::pair<std::string, std::size_t>> pinRefTable;

        std::getline(file, line); // Skip "componentsData"
        while (std::getline(file, line) && line != "end") {
            std::istringstream iss(line);
            std::string componentName, type, truthTableName, rest;
            iss >> componentName >> type >> truthTableName;
            std::getline(iss, rest);

            std::vector<std::size_t> inputs = {1, 2}, outputs = {3};
            size_t start = rest.find('{');
            size_t end = rest.find('}');
            if (start != std::string::npos && end != std::string::npos) {
                inputs.clear();
                std::string inputsStr = rest.substr(start + 1, end - start - 1);
                std::istringstream issInputs(inputsStr);
                std::string input;
                while (std::getline(issInputs, input, ',')) {
                    inputs.push_back(std::stoi(input));
                }
            }

            start = rest.find('{', end + 1);
            end = rest.find('}', end + 1);
            if (start != std::string::npos && end != std::string::npos) {
                outputs.clear();
                std::string outputsStr = rest.substr(start + 1, end - start - 1);
                std::istringstream issOutputs(outputsStr);
                std::string output;
                while (std::getline(issOutputs, output, ',')) {
                    outputs.push_back(std::stoi(output));
                }
            }

            componentsData.push_back({componentName, ComponentType::Standard, std::get<3>(_componentTruthTables[truthTableName]), inputs, outputs});
        }

        std::getline(file, line); // Skip "pinRefTable"
        while (std::getline(file, line) && line != "end") {
            std::istringstream iss(line);
            std::size_t pin;
            std::string componentName;
            std::size_t componentPin;
            iss >> pin >> componentName >> componentPin;
            pinRefTable[pin] = {componentName, componentPin};
        }

        _gates[gateName] = [this, pinNb, componentsData, pinRefTable](const std::string &label) {
            return new GenericGate(label, pinNb, componentsData, pinRefTable);
        };
    }
}
