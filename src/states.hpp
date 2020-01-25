#ifndef STATES_HPP 
#define STATES_HPP
#include <iostream>
#include "machine.hpp"

struct state {
    virtual void perform(machine<int> &m) = 0;
    virtual std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                                      const machine<int> &) = 0;
    virtual ~state() = default;
};

class incr_state : public state {
public:
    void perform(machine<int> &m) final override { m.increment_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

class decr_state : public state {
public:
    void perform(machine<int> &m) final override { m.decrement_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

class next_state : public state {
public:
    void perform(machine<int> &m) final override { m.next_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

class prev_state : public state {
public:
    void perform(machine<int> &m) final override { m.previous_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

class output_state : public state {
public:
    void perform(machine<int> &m) final override { fmt::print("{}\n", m.output_current()); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

class start_loop_state : public state {
public:
    void perform(machine<int> &) final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &s,
                                              const machine<int> &m) final override {

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
    void perform(machine<int> &) final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &s,
                                              const machine<int> &) final override {

        return std::find(std::cbegin(s), it, '[');
    }
};

class input_state : public state {
public:
    void perform(machine<int> &m) final override {
        fmt::print("Data to input:\n>\t");
        auto input = 0;
        std::cin >> input;
        m.input_current(input);
    }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &,
                                              const machine<int> &) final override {
        return std::next(it);
    }
};

#endif
