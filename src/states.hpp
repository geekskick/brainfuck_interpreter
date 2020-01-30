#ifndef STATES_HPP
#define STATES_HPP
#include "fmt/format.h"
#include "machine.hpp"
#include <algorithm>
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
        if(!m.current_is_zero()){
            return std::next(it);
        }

        const auto eol_pos = find_matching_brace(s, std::distance(s.cbegin(), it));
        const auto eol_it = [&eol_pos, &s]() {
            if (std::string::npos == eol_pos) {
                fmt::print("No Matching end of loop in current line, that's bad!\n");
                return s.cend() - 1;
            }
            return s.cbegin() + eol_pos;
        }();

        return std::next(eol_it);
    }

private:
    template<typename SType>
    size_t find_matching_brace(const SType &s, const size_t brace_pos) const {
        auto n_between = 0;
        auto eol = brace_pos;
        do {
            eol = s.find("]", eol + 1);
            if(eol == std::string::npos){break;}
            n_between = std::count_if(s.cbegin() + brace_pos + 1, s.cbegin() + eol,
                                      [](const char c) { return c == ']' || c == '['; });
        } while (n_between % 2 != 0);
        return eol;
    }
};

class end_loop_state : public state {
public:
    void perform(machine<int> &) const final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &s,
                                              const machine<int> &) const final override {
        const auto sol_pos = find_matching_brace(s, std::distance(s.cbegin(), it));
        const auto sol_it = [&sol_pos, &s]() {
            if (std::string::npos == sol_pos) {
                fmt::print("No Start of loop in current line, that's bad!\n");
                return s.cend();
            }
            return s.cbegin() + sol_pos;
        }();
        return sol_it;
    }

private:
    template <typename SType> size_t find_matching_brace(const SType &s, const size_t brace_pos) const {
        auto n_between = 0;
        auto sol = brace_pos;
        do {
            sol = s.rfind("[", sol - 1);
            n_between = std::count_if(s.cbegin() + sol + 1, s.cbegin() + brace_pos,
                                      [](const char c) { return c == ']' || c == '['; });
        } while (n_between % 2 != 0);
        return sol;
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
