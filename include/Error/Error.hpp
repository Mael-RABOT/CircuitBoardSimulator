#pragma once

#include <exception>

namespace nts {
    class Error : public std::exception {
        private:
            std::string _message;

        public:
            Error(const std::string &message) : _message(message) {};
            const char *what() const noexcept override {
                return _message.c_str();
            }
    };
}