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
            const std::string &label
    ) {
        if (type == "input")
            return this->addComponent(label, new nts::Input());
        else if (type == "output")
            return this->addComponent(label, new nts::Output());
        else if (type == "clock")
            return this->addComponent(label, new nts::Clock());
        else if (type == "true")
            return this->addComponent(label, new TrueC());
        else if (type == "false")
            return this->addComponent(label, new FalseC());
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

    void Manager::debug(bool inputs, bool components, bool outputs) const {
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

    void Manager::_stageChipsetHandler(const std::string &line) {
        std::istringstream iss(line);
        std::string type, label;

        if (!(iss >> type && iss >> label)) {
            throw CustomError("Invalid chipset line: " + line);
        }
        label = label.substr(0, label.find_first_of("#"));
        label.erase(label.find_last_not_of(" \t\r\n") + 1);
        if (!this->factory(type, label)) {
            throw CustomError("Could not add component: " + label);
        }
    }

    void Manager::_stageLinksHandler(const std::string &line) {
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

    void Manager::_parserLoop(std::ifstream &fs) {
        std::string line;
        nts::ParserStage stage = nts::ParserStage::UNDEFINED;
        while (std::getline(fs, line)) {
            stage = stageChecker(fs, line, stage);
            if (line.empty() || line[0] == '#')
                continue;
            switch (stage) {
                case nts::ParserStage::CHIPSET:
                    this->_stageChipsetHandler(line);
                    break;
                case nts::ParserStage::LINKS:
                    this->_stageLinksHandler(line);
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
        if (std::string(av[1]) == "--run-empty") {
            this->factory("input", "input1");
            this->factory("output", "output1");
            this->addLink("input1", 1, *this->getComponent("output1"), 1);
            return;
        }
        const char* extension = strrchr(av[1], '.');
        if (extension == nullptr || strcmp(extension, ".nts") != 0) {
            throw CustomError("Invalid file extension: " + std::string(av[1]));
        }
        std::ifstream fs(av[1]);
        if (!fs.is_open()) {
            throw CustomError("Could not open file: " + std::string(av[1]));
        }
        this->_parserLoop(fs);
        fs.close();
    }

    void Manager::display() {
        std::cout << "tick: " << _lastTick << std::endl;
        std::cout << "input(s):" << std::endl;
        for (auto &input: _allChip) {
            if (input.second->getType() != ComponentType::INPUT)
                continue;
            std::cout << "  " << input.first << ": "
                << ((input.second->getPin(1).getState() == nts::Tristate::Undefined)
                    ? "U" : (input.second->getPin(1).getState() == nts::Tristate::True) ? "1" : "0")
                << std::endl;
        }
        std::cout << "output(s):" << std::endl;
        for (auto &output: _allChip) {
            if (output.second->getType() != ComponentType::OUTPUT)
                continue;
            std::cout << "  " << output.first << ": "
                << ((output.second->getPin(1).getState() == nts::Tristate::Undefined)
                    ? "U" : (output.second->getPin(1).getState() == nts::Tristate::True) ? "1" : "0")
                << std::endl;
        }
    }

    void Manager::_handleCommand(const std::string &line) {
        if (line == "clear") {
            #ifdef __unix__
            system("clear");
            #endif
            #ifdef _WIN32
            throw CustomError("Don't use Windows");
            #endif
            return;
        }
        if (line == "debug") {
            this->debug();
            return;
        }
        if (line == "simulate") {
            this->simulate();
            return;
        }
        if (line == "display") {
            this->display();
            return;
        }
        if (line.find("add ") == 0) {
            std::string command = line.substr(4);
            try {
                this->_stageChipsetHandler(command);
            } catch (const CustomError &e) {
                std::cout << "---- " << e.what() << std::endl;
            }
            return;
        }
        if (line.find("link ") == 0) {
            std::string command = line.substr(5);
            try {
                this->_stageLinksHandler(command);
            } catch (const CustomError &e) {
                std::cout << "---- " << e.what() << std::endl;
            }
            return;
        }
        this->_interpretLine(line);
    }

    void Manager::_interpretLine(const std::string &line) {
        std::string target;
        std::string tension;

        std::istringstream iss(line);
        if (!(std::getline(iss, target, '=') && std::getline(iss, tension) && iss.eof())) {
            throw CustomError("Invalid command: " + line);
        }
        if (tension != "0" && tension != "1") {
            throw CustomError("Invalid tension: " + tension);
        }
        if (_allChip.find(target) == _allChip.end()) {
            throw CustomError("Unknown component: " + target);
        }
        if (_allChip[target]->getType() != ComponentType::INPUT) {
            throw CustomError("Invalid component type: " + target);
        }
        _allChip[target]->getPin(1).setState(
                (tension == "1") ? nts::Tristate::True :
                nts::Tristate::False
        );
    }

    void Manager::_checkRun() const {
        bool hasInput = false;
        for (auto &component: _allChip) {
            if (component.second->getType() == ComponentType::INPUT) {
                hasInput = true;
                break;
            }
        }
        if (!hasInput) {
            throw CustomError("No input found");
        }
        bool hasOutput = false;
        for (auto &component: _allChip) {
            if (component.second->getType() == ComponentType::OUTPUT) {
                hasOutput = true;
                break;
            }
        }
        if (!hasOutput) {
            throw CustomError("No output found");
        }
    }

    void Manager::run() {
        this->_checkRun();
        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line)) {
            std::vector<std::string> commands;
            std::istringstream iss(line);
            std::string token;

            while (std::getline(iss, token, '&')) {
                token.erase(0, token.find_first_not_of(" \t\r\n"));
                token.erase(token.find_last_not_of(" \t\r\n") + 1);
                if (!token.empty()) {
                    commands.push_back(token);
                }
            }

            for (const auto& command : commands) {
                if (command == "exit") {
                    return;
                }
                this->_handleCommand(command);
            }
        }
    }
}
