#pragma once

#include <cstring>
#include <fstream>
#include <sstream>

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

            void debug() const { this->debug(true, true, true); };
            void debug(bool, bool, bool) const;

            void simulate() { this->simulate(_lastTick + 1); };
            void simulate(std::size_t tick);

            void stageLinksHandler(const std::string &line);
            void parserLoop(std::ifstream &fs);
            void parser(int ac, char **av);

            IComponent *getComponent(const std::string &label) {
                if (_allChip.find(label) != _allChip.end())
                    return _allChip[label];
                throw CustomError("Unknown component: " + label);
            };
    };
}
