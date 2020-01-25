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

static const auto state_map = []() {
    auto rc = std::map<char, std::unique_ptr<state>>();
    rc[symbols::increment] = std::make_unique<incr_state>();
    rc[symbols::decrement] = std::make_unique<decr_state>();
    rc[symbols::output] = std::make_unique<output_state>();
    rc[symbols::input] = std::make_unique<input_state>();
    rc[symbols::start_loop] = std::make_unique<start_loop_state>();
    rc[symbols::end_loop] = std::make_unique<end_loop_state>();
    return rc;
}();

int main(const int argc, const char *argv[]) {
    if (help_arg(argc, argv)) {
        print_usage();
    }

    fmt::print("Continuing as normal, [Ctrl-C or {}] to quit.\n", commands::quit);

    auto m = machine<int>{};

    while (1) {
        using namespace std::literals;
        auto input = ""s;
        std::getline(std::cin, input);
        for (auto c = std::cbegin(input); c != std::cend(input); c = state_map.at(*c)->next_iterator(c, input, m)) {
            state_map.at(*c)->perform(m);
        }
    }

    return EXIT_FAILURE;
}
