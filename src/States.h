#ifndef STATES_H
#define STATES_H

#include <Arduino.h>
#include <State.h>
#include <analog_out.h>

class StateVariables {
public:
    bool pre;
    bool op;
    bool flt;
    char c;
    Analog_out led;

    StateVariables() : led(5) {} // Initialize the analog_out in the constructor
};

class Initialization : public State {
    public:
    StateVariables stateVars;

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};
class PreOperational : public State {
    public:
    StateVariables stateVars;

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

class Operational : public State {
    public:
    StateVariables stateVars;

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

class Stopped : public State {
    public:
    StateVariables stateVars;

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

#endif