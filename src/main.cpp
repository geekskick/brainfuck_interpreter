#include "fmt/format.h"
#include "machine.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace {
[[noreturn]] void print_usage() {
    fmt::print("This is a brainfuck interpreter, currently you just type in brainfuck stuff then a newline to get it "
               "to evaluate\n");
    exit(EXIT_SUCCESS);
}
bool help_arg(const int argc, const char *argv[]) {

    using namespace std::literals;
    return (argc > 1 && (argv[1] == "--help"sv || argv[1] == "-h"sv));
}
} // namespace

struct symbols {
    constexpr static auto increment = '+';
    constexpr static auto decrement = '-';
    constexpr static auto next = '>';
    constexpr static auto prev = '<';
    constexpr static auto output = '.';
    constexpr static auto input = ',';
    constexpr static auto start_loop = '[';
    constexpr static auto end_loop = ']';
};

struct commands {
    constexpr static auto quit = 'q';
};

struct state {
    virtual void perform(machine<int> &m) = 0;
    virtual std::string::const_iterator next_iterator(const std::string::const_iterator &it, const std::string &) = 0;
    virtual ~state() = default;
};

class incr_state : public state {
public:
    void perform(machine<int> &m) final override { m.increment_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &) final override {
        return std::next(it);
    }
};

class decr_state : public state {
public:
    void perform(machine<int> &m) final override { m.decrement_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &) final override {
        return std::next(it);
    }
};

class next_state : public state {
public:
    void perform(machine<int> &m) final override { m.next_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &) final override {
        return std::next(it);
    }
};

class prev_state : public state {
public:
    void perform(machine<int> &m) final override { m.previous_data(); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &) final override {
        return std::next(it);
    }
};

class output_state : public state {
public:
    void perform(machine<int> &m) final override { fmt::print("{}\n", m.output_current()); }
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &) final override {
        return std::next(it);
    }
};

class loop_state : public state {
public:
    void perform(machine<int> &m) final override {}
    std::string::const_iterator next_iterator(const std::string::const_iterator &it,
                                              const std::string &s) final override {
        auto rc = std::next(it);
        if (const auto end = std::find(it, std::cend(s), ']'); end != std::cend(s)) {
            if (m.current_is_zero()) {

            } else {

            }
        } else {
            fmt::print("Not end of loop in the current line, that's bad! Exitting\n");
            return s.cend();
        }
        return rc;
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
    std::string::const_iterator next_iterator(const std::string::const_iterator &it) final override {
        return std::next(it);
    }
};

static const std::map<char, std::unique_ptr<state>> state_map = []() {
    auto rc = std::map<char, std::unique_ptr<state>>();
    rc[symbols::increment] = std::make_unique<incr_state>();
    rc[symbols::decrement] = std::make_unique<decr_state>();
    rc[symbols::output] = std::make_unique<output_state>();
    rc[symbols::input] = std::make_unique<input_state>();
    rc[symbols::start_loop] = std::make_unsigned<loop_state>();
    return rc;
}();

int main(const int argc, const char *argv[]) {
    if (help_arg(argc, argv)) {
        print_usage();
    }

    fmt::print("Continuing as normal, [Ctrl-C or {}] to quit.\n", commands::quit);

    auto m = machine<int>{};
    //    auto start_of_loop = std::string::const_iterator{};

    while (1) {
        using namespace std::literals;
        auto input = ""s;
        std::getline(std::cin, input);
        for (auto c = std::cbegin(input); c != std::cend(input); c = state_map.at(*c)->next_iterator(c)) {
            state_map.at(*c)->perform(m);
        }
        /*
    case symbols::increment:
        m.increment_data();
        c++;
        break;
    case symbols::decrement:
        m.decrement_data();
        c++;
        break;
    case symbols::next:
        m.next_data();
        c++;
        break;
    case symbols::prev:
        m.previous_data();
        c++;
        break;
    case symbols::output:
        fmt::print("{}\n", m.output_current());
        c++;
        break;
    case symbols::start_loop:
        if (const auto end = std::find(c, std::cend(input), ']'); end != std::cend(input)) {
            if (m.current_is_zero()) {
                c = end + 1;
                start_of_loop = std::string::const_iterator{};
            } else {
                start_of_loop = c;
                c++;
            }
        } else {
            fmt::print("Not end of loop in the current line, that's bad! Exitting this line\n");
            break;
        }
        break;
    case symbols::end_loop:
        c = start_of_loop;
        break;
    case symbols::input: {
        fmt::print("Data to input:\n>\t");
        auto input = 0;
        std::cin >> input;
        m.input_current(input);
        c++;
    } break;
    case commands::quit:
        fmt::print("Exitting\n");
        return EXIT_SUCCESS;
        break;
    default:
        fmt::print("{} isn't supported\n", *c);
        break;
    }
}
*/
    }

    return EXIT_FAILURE;
}
