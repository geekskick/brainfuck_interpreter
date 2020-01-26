#ifndef STATES_HPP
#define STATES_HPP
#include "fmt/format.h"
#include "machine.hpp"
#include <iostream>

struct state {
    virtual void perform(machine<int> &m) const = 0;
    virtual std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                                      const machine<int> &) const = 0;
    virtual ~state() = default;
};

class incr_state : public state {
public:
    void perform(machine<int> &m) const final override { m.increment_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

class decr_state : public state {
public:
    void perform(machine<int> &m) const final override { m.decrement_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

class next_state : public state {
public:
    void perform(machine<int> &m) const final override { m.next_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

class prev_state : public state {
public:
    void perform(machine<int> &m) const final override { m.previous_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

class output_state : public state {
public:
    void perform(machine<int> &m) const final override { fmt::print("{}\n", m.output_current()); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

class start_loop_state : public state {
public:
    void perform(machine<int> &) const final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &s,
                                              const machine<int> &m) const final override {

        if (const auto end = std::find(it, std::cend(s), ']'); end != std::cend(s)) {
            if (m.current_is_zero()) {
                return std::next(end);
            }
            return std::next(it);
        }
        fmt::print("Not end of loop in the current line, that's bad! Exitting\n");
        return s.cend();
    }
};

class end_loop_state : public state {
public:
    void perform(machine<int> &) const final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &s,
                                              const machine<int> &) const final override {

        if (const auto n = std::find(std::cbegin(s), it, '['); n != it) {
            return n;
        }
        fmt::print("No start of loop in current line, that's bad! Exitting\n");
        return std::cend(s);
    }
};

class input_state : public state {
public:
    void perform(machine<int> &m) const final override {
        fmt::print("Data to input:\n>\t");
        auto input = 0;
        std::cin >> input;
        m.input_current(input);
    }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) const final override {
        return std::next(it);
    }
};

#endif
