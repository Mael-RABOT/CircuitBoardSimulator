#include "Manager.hpp"

namespace nts {
    Manager::~Manager() {
        for (auto &component: _allChip) {
            delete component.second;
        }
    }

    bool Manager::addComponent(const std::string &label, IComponent *component) {
        if (_allChip.find(label) != _allChip.end())
            return false;
        _allChip[label] = component;
        return true;
    }

    bool Manager::factory(
            const std::string &type,
            const std::string &label,
            nts::Tristate state = nts::Tristate::Undefined
    ) {
        if (type == "input")
            return this->addComponent(label, new nts::Input(state));
        else if (type == "clock")
            return this->addComponent(label, new nts::Clock(state));
        else if (type == "output")
            return this->addComponent(label, new nts::Output());
        else if (type == "and")
            return this->addComponent(label, new nts::And());
        throw CustomError("Unknown component: " + type);
    }

    void Manager::addLink(
            const std::string &label,
            std::size_t sourcePin,
            nts::IComponent &other,
            std::size_t otherPin
    ) {
        _allChip[label]->setLink(sourcePin, other, otherPin);
    }

    void Manager::debug(
            bool inputs __attribute_maybe_unused__ = true,
            bool components __attribute_maybe_unused__ = true,
            bool outputs __attribute_maybe_unused__ = true
    ) const {
        if (inputs) {
            std::cout << "Inputs: " << std::endl;
            for (auto &input: _allChip) {
                if (input.second->getType() != ComponentType::INPUT)
                    continue;
                std::cout << " - " << input.first << std::endl;
                for (auto &pin: input.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
        if (components) {
            std::cout << std::endl << "Components: " << std::endl;
            for (auto &component: _allChip) {
                if (component.second->getType() != ComponentType::COMPONENT)
                    continue;
                std::cout << " - " << component.first << std::endl;
                for (auto &pin: component.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
        if (outputs) {
            std::cout << std::endl << "Outputs: " << std::endl;
            for (auto &output: _allChip) {
                if (output.second->getType() != ComponentType::OUTPUT)
                    continue;
                std::cout << " - " << output.first << std::endl;
                for (auto &pin: output.second->getPins()) {
                    std::cout << "   - pin " << pin.first << " with state: " << pin.second.getState() << std::endl;
                }
            }
        }
    }

    void Manager::simulate(std::size_t tick) {
        _lastTick = tick;
        for (auto &output : _allChip) {
            if (output.second->getType() == ComponentType::OUTPUT)
                output.second->simulate(tick);
        }
    }

    nts::ParserStage stageChecker(std::ifstream &fs, std::string &line, nts::ParserStage &stage) {
        nts::ParserStage newStage = nts::ParserStage::UNCHANGED;
        if (line == ".chipsets:") {
            newStage = nts::ParserStage::CHIPSET;
            std::getline(fs, line);
        } else if (line == ".links:") {
            newStage = nts::ParserStage::LINKS;
            std::getline(fs, line);
        }
        if (newStage != nts::ParserStage::UNCHANGED && newStage < stage) {
            throw CustomError("Invalid stage: " + std::to_string(newStage));
        }
        return newStage = (newStage == nts::ParserStage::UNCHANGED) ? stage : newStage;
    }

    void stageChipsetHandler(nts::Manager &manager, const std::string &line) {
        std::istringstream iss(line);
        std::string type, label;

        if (!(iss >> type && iss >> label)) {
            throw CustomError("Invalid chipset line: " + line);
        }
        if (!manager.factory(type, label)) {
            throw CustomError("Could not add component: " + label);
        }
    }

    void Manager::stageLinksHandler(const std::string &line) {
        std::istringstream iss(line);
        std::string src, dest;

        if (!(iss >> src && iss >> dest)) {
            throw CustomError("Invalid link line: " + line);
        }
        std::size_t srcPin, destPin;
        try {
            srcPin = std::stoi(src.substr(src.find(":") + 1));
            destPin = std::stoi(dest.substr(dest.find(":") + 1));
        } catch (const std::invalid_argument& e) {
            throw CustomError("Invalid argument: " + std::string(e.what()));
        } catch (const std::out_of_range& e) {
            throw CustomError("Out of range: " + std::string(e.what()));
        }
        src = src.substr(0, src.find(":"));
        dest = dest.substr(0, dest.find(":"));

        bool srcExist = false;
        if (_allChip.find(src) != _allChip.end())
            srcExist = true;
        if (!srcExist)
            throw CustomError("Source Component not found: " + src);

        bool destExist = false;
        if (_allChip.find(dest) != _allChip.end())
            destExist = true;
        if (!destExist)
            throw CustomError("Destination Component not found: " + dest);

        this->getComponent(src);

        this->addLink(
                src,
                srcPin,
                *this->getComponent(dest),
                destPin);
    }

    void Manager::parserLoop(std::ifstream &fs) {
        std::string line;
        nts::ParserStage stage = nts::ParserStage::UNDEFINED;
        while (std::getline(fs, line)) {
            stage = stageChecker(fs, line, stage);
            if (line.empty() || line[0] == '#')
                continue;
            switch (stage) {
                case nts::ParserStage::CHIPSET:
                    stageChipsetHandler(*this, line);
                    break;
                case nts::ParserStage::LINKS:
                    this->stageLinksHandler(line);
                    break;
                default:
                    throw CustomError("Invalid stage: " + std::to_string(stage));
            }
        }
    }

    void Manager::parser(int ac, char **av) {
        if (ac != 2 || std::string(av[1]) == "-h" || std::string(av[1]) == "--help") {
            throw CustomError("Usage: " + std::string(av[0]) + " <filename>");
        }
        const char* extension = strrchr(av[1], '.');
        if (extension == nullptr || strcmp(extension, ".nts") != 0) {
            throw CustomError("Invalid file extension: " + std::string(av[1]));
        }
        std::ifstream fs(av[1]);
        if (!fs.is_open()) {
            throw CustomError("Could not open file: " + std::string(av[1]));
        }
        this->parserLoop(fs);
        fs.close();
    }
}
