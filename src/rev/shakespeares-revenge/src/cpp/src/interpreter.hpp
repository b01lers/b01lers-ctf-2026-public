#ifndef SHAKESPEARE_CPP_INTERPRETER_HPP
#define SHAKESPEARE_CPP_INTERPRETER_HPP

#include "model.hpp"

#include <string>

class ShakespeareInterpreter {
public:
    ShakespeareInterpreter(const Play& play, const std::string& input_style, const std::string& output_style);
    ~ShakespeareInterpreter();
    ShakespeareInterpreter(const ShakespeareInterpreter&) = delete;
    ShakespeareInterpreter& operator=(const ShakespeareInterpreter&) = delete;
    void run();

private:
    Play play_;
    std::string input_style_;
    std::string output_style_;

    struct Impl;
    Impl* impl_;
};

#endif
