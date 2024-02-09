#include "Manager.hpp"

namespace nts {

    bool g_sigintCaught = false;

    bool Manager::createComponent(const std::string &type, const std::string &label) {
        return this->_factory(type, label);
    }

    bool Manager::_addComponent(const std::string &label, IComponent *component) {
        if (component == nullptr)
            throw CustomError("Creation went wrong.");
        if (_components.find(label) != _components.end())
            return false;
        _components[label] = std::move(component);
        return true;
    }

    bool Manager::_factory(const std::string &type, const std::string &label) {
        std::map<std::string, std::pair<int, std::vector<std::vector<nts::Tristate>>>> logicGateTypes = {
                {"and", {3, nts::AND_TRUTH_TABLE}},
                {"or", {3, nts::OR_TRUTH_TABLE}},
                {"xor", {3, nts::XOR_TRUTH_TABLE}},
                {"not", {2, nts::NOT_TRUTH_TABLE}},
                {"nand", {3, nts::NAND_TRUTH_TABLE}}
        };

        std::map<std::string, std::function<IComponent*(const std::string&)>> special = {
                {"input", [](const std::string &label) { return new InputComp(label); }},
                {"output", [](const std::string &label) { return new OutputComp(label); }},
                {"clock", [](const std::string &label) { return new Clock(label); }},
                {"true", [](const std::string &label) { return new TrueConst(label); }},
                {"false", [](const std::string &label) { return new FalseConst(label); }}
        };

        if (special.find(type) != special.end()) {
            return this->_addComponent(label, special[type](label));
        }
        if (logicGateTypes.find(type) != logicGateTypes.end()) {
            return this->_addComponent(label, new LogicGate(logicGateTypes[type].first, label, ComponentType::Standard, logicGateTypes[type].second));
        }
        throw CustomError("Invalid component type: " + type);
    }

    void Manager::addLink(
            const std::string &source,
            std::size_t sourcePin,
            const std::string &dest,
            std::size_t otherPin
    ) { // This function DOES NOT link back if both components are outputs
        _components[dest]->setLink(otherPin, *_components[source], sourcePin);
        if (_components[source]->getType() == ComponentType::Output && _components[dest]->getType() == ComponentType::Output)
            return;
        _components[source]->setLink(sourcePin, *_components[dest], otherPin);
    }

    void Manager::removeLink(
        const std::string &source,
        std::size_t sourcePin,
        const std::string &dest,
        std::size_t otherPin
    ) {
        _components[source]->removeLink(sourcePin);
        _components[dest]->removeLink(otherPin);
    }

    void Manager::_dumpPrint(const std::string &title, ComponentType type) {
        std::cout << title << std::endl;
        for (auto &input : _components) {
            if (input.second->getType() != type)
                continue;
            input.second->dump();
        }
    }

    void Manager::dump(bool inputs, bool components, bool outputs) {
        std::cout << "tick: " << _currentTick << std::endl;
        if (inputs) {
            this->_dumpPrint("Inputs: ", ComponentType::Input);
        }
        if (components) {
            this->_dumpPrint("Standards: ", ComponentType::Standard);
        }
        if (outputs) {
            this->_dumpPrint("Output: ", ComponentType::Output);
        }
    }

    void Manager::simulate(std::size_t tick) {
        _currentTick = tick;
        for (auto &output : _components) {
            if (output.second->getType() == ComponentType::Input)
                output.second->computeBehaviour(tick);
        }
        for (auto &output : _components) {
            if (output.second->getType() == ComponentType::Standard)
                output.second->computeBehaviour(tick);
        }
        for (auto &output : _components) {
            if (output.second->getType() == ComponentType::Output)
                output.second->computeBehaviour(tick);
        }
    }

    nts::ParserStage Manager::stageChecker(std::ifstream &fs, std::string &line, nts::ParserStage &stage) {
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
        if (!this->createComponent(type, label)) {
            throw CustomError("Component already exist: " + label);
        }
    }

    void Manager::_stageLinksHandler(const std::string &line, bool remove) {
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
        if (_components.find(src) != _components.end())
            srcExist = true;
        if (!srcExist)
            throw CustomError("Source Component not found: " + src);

        bool destExist = false;
        if (_components.find(dest) != _components.end())
            destExist = true;
        if (!destExist)
            throw CustomError("Destination Component not found: " + dest);

        if (!remove)
            return this->addLink(
                src,
                srcPin,
                dest,
                destPin);

        this->removeLink(
            src,
            srcPin,
            dest,
            destPin);
    }

    void Manager::_parserLoop(std::ifstream &fs) {
        std::string line;
        nts::ParserStage stage = nts::ParserStage::UNDEFINED;
        while (std::getline(fs, line)) {
            stage = Manager::stageChecker(fs, line, stage);
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

    void Manager::_help() {
        if (std::system("pandoc --version > /dev/null 2>&1") != 0) {
            throw CustomError("Usage: ./nanotekspice [file.nts] (flags)");
        }
        if (std::system("lynx --version > /dev/null 2>&1") != 0) {
            throw CustomError("Usage: ./nanotekspice [file.nts] (flags)");
        }
        system("pandoc ReadMe.md | lynx -stdin");
    }

    void Manager::parser(int ac, char **av) {
        if (ac <= 1) throw CustomError("Usage: ./nanotekspice [file.nts] (flags)");
        if (std::string(av[1]) == "-h" || std::string(av[1]) == "--help") {
            _help();
            exit(0);
        }
        if (std::string(av[1]) == "--run-empty") {
            this->createComponent("input", "input1");
            this->createComponent("output", "output1");
            this->addLink("input1", 1, "output1", 1);
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

    void Manager::_displayPrint(const std::string &title, ComponentType type) {
        std::cout << title << std::endl;
        for (auto &input: _components) {
            if (input.second->getType() != type)
                continue;
            std::cout << "  " << input.first << ": "
                << ((input.second->getPins()[1].first == nts::Tristate::Undefined)
                ? "U" : (input.second->getPins()[1].first == nts::Tristate::True) ? "1" : "0")
                << std::endl;
        }
    }

    void Manager::display() {
        std::cout << "tick: " << _currentTick << std::endl;
        this->_displayPrint("input(s):", ComponentType::Input);
        this->_displayPrint("output(s):", ComponentType::Output);
    }

    void Manager::_loop() {
        signal(SIGINT, [](int) {
            std::cout << std::endl; // Prevents the ^C from being printed
            g_sigintCaught = true;
        });

        while (!g_sigintCaught) {
            this->simulate(_currentTick + 1);
            this->display();
        }
    }

    void Manager::_handleCommand(const std::string &line) {
        std::map<std::string, std::function<void()>> commands = {
                {"clear", [this]() {
                    #ifdef __unix__
                    system("clear");
                    #endif
                    #ifdef _WIN32
                    throw CustomError("Use a real operating system");
                    #endif
                }},
                {"loop", [this]() { this->_loop(); }},
                {"dump", [this]() { this->dump(); }},
                {"simulate", [this]() { this->simulate(_currentTick + 1); }},
                {"sm", [this]() { this->simulate(_currentTick + 1); }},
                {"display", [this]() { this->display(); }},
                {"ls", [this]() { this->display(); }},
                {"add ", [this, &line]() {
                    std::string command = line.substr(4);
                    this->_stageChipsetHandler(command);
                }},
                {"link ", [this, &line]() {
                    std::string command = line.substr(5);
                    this->_stageLinksHandler(command);
                }},
                {"removeLink ", [this, &line]() {
                    std::string command = line.substr(11);
                    this->_stageLinksHandler(command, true);
                }},
                {"removeChipset ", [this, &line]() {
                    std::string command = line.substr(14);
                    if (_components.find(command) == _components.end()) {
                        throw CustomError("Component not found: " + command);
                    }
                    _components.erase(command);
                }}
        };

        for (auto &command : commands) {
            if (line.find(command.first) == 0) {
                command.second();
                return;
            }
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
        if (_components.find(target) == _components.end()) {
            throw CustomError("Unknown component: " + target);
        }
        if (_components[target]->getType() != ComponentType::Input) {
            throw CustomError("Invalid component type: " + target);
        }
        _components[target]->setState(1, (tension == "1") ? Tristate::True : False);
    }

    void Manager::_checkRun() const {
        if (_components.empty()) {
            throw CustomError("No components found");
        }
        bool hasInput = false;
        for (auto &component: _components) {
            if (component.second->getType() == ComponentType::Input) {
                hasInput = true;
                break;
            }
        }
        if (!hasInput) {
            throw CustomError("No input found");
        }
        bool hasOutput = false;
        for (auto &component: _components) {
            if (component.second->getType() == ComponentType::Output) {
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
                try {
                    this->_handleCommand(command);
                } catch (const CustomError &e) {
                    std::cout << "\033[31m" << e.what() << "\033[39m" << std::endl;
                }
            }
        }
    }
}
