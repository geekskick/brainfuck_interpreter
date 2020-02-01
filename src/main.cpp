#include "fmt/format.h"
#include "machine.hpp"
#include "states.hpp"
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace {
[[noreturn]] void print_usage() {
    fmt::print("This is a brainfuck interpreter, currently you just type in brainfuck stuff then a newline to get it "
               "to evaluate\n");
    fmt::print("--help,-h    show this message\n");
    fmt::print("--slow,-s    run through in slow time\n");
    exit(EXIT_SUCCESS);
}
bool help_arg(const int argc, const char *argv[]) {
    using namespace std::literals;
    return (argc > 1 && (argv[1] == "--help"sv || argv[1] == "-h"sv));
}
bool slow_arg(const int argc, const char *argv[]) {
    using namespace std::literals;
    for (int i = 0; i < argc; i++) {
        if (std::string{argv[i]} == "--slow" || std::string{argv[i]} == "-s") {
            return true;
        }
    }
    return false;
}
void wait_for_user_input() {
    using namespace std::literals;
    auto waste = ""s;
    std::getline(std::cin, waste);
}

void show_command(const std::string &input, const machine<int> &m, const std::string::const_iterator &c) {
    fmt::print("{}\n", input);
    auto indicator = std::string(input.size(), ' ');
    indicator[std::distance(input.cbegin(), c)] = '^';
    fmt::print("{}\n", indicator);
    fmt::print("Mem: {}\n", m.get_memory());
}

std::string get_user_input() {
    using namespace std::literals;
    auto input = ""s;
    std::getline(std::cin, input);
    return input;
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

static const auto state_map = []() {
    auto rc = std::map<char, std::unique_ptr<state>>();
    rc[symbols::increment] = std::make_unique<incr_state>();
    rc[symbols::decrement] = std::make_unique<decr_state>();
    rc[symbols::output] = std::make_unique<output_state>();
    rc[symbols::input] = std::make_unique<input_state>();
    rc[symbols::prev] = std::make_unique<prev_state>();
    rc[symbols::next] = std::make_unique<next_state>();
    rc[symbols::start_loop] = std::make_unique<start_loop_state>();
    rc[symbols::end_loop] = std::make_unique<end_loop_state>();
    return rc;
}();

int main(const int argc, const char *argv[]) {
    if (help_arg(argc, argv)) {
        print_usage();
    }
    const auto slow_time = slow_arg(argc, argv);

    fmt::print("Continuing as normal, [Ctrl-C or {}] to quit.\n", commands::quit);

    auto m = machine<int>{};

    while (1) {
        const auto input = get_user_input();
        for (auto c = std::cbegin(input); c != std::cend(input);) {
            if (const auto n = state_map.find(*c); n != state_map.cend()) {

                n->second->perform(m);

                if (slow_time) {
                    show_command(input, m, c);
                }

                c = n->second->next_iterator(c, input, m);

                if (slow_time) {
                    wait_for_user_input();
                }

            } else if (*c == commands::quit) {
                fmt::print("Detected {}, so I'm stopping\n", *c);
                fmt::print("Thanks for playing\n");
                return EXIT_SUCCESS;
            } else {
                fmt::print("Ignoring invalid character {}\n", *c);
                c++;
            }
        }
    }
}
