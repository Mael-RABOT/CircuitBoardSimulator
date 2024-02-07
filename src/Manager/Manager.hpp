#pragma once

#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "../../include/Components.hpp"

namespace nts {
    enum ParserStage {
        UNDEFINED = -1,
        UNCHANGED = 0,
        CHIPSET = 1,
        LINKS = 2
    };

    class Manager {
        private:
            std::map<std::string, IComponent *> _allChip;
            std::size_t _lastTick;

            void _handleCommand(const std::string &line);
            void _interpretLine(const std::string &line);

            void _stageChipsetHandler(const std::string &line);
            void _stageLinksHandler(const std::string &line);
            void _parserLoop(std::ifstream &fs);

            void _checkRun() const;

        public:
            Manager() : _lastTick(0) {};
            ~Manager();

            bool addComponent(const std::string &label, IComponent *component);
            bool factory(
                const std::string &type,
                const std::string &label,
                nts::Tristate state);

            void addLink(
                    const std::string &label,
                    std::size_t sourcePin,
                    nts::IComponent &other,
                    std::size_t otherPin);

            void debug(bool = true, bool = true, bool = true) const;

            void simulate() { this->simulate(_lastTick + 1); };
            void simulate(std::size_t tick);

            void parser(int ac, char **av);

            IComponent *getComponent(const std::string &label) {
                if (_allChip.find(label) != _allChip.end())
                    return _allChip[label];
                throw CustomError("Unknown component: " + label);
            };

            void display();
            void run();
    };
}
