#include "fmt/format.h"
#include "machine.hpp"
#include <iostream>
#include <string>
#include <string_view>

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

int main(const int argc, const char *argv[]) {
    if (help_arg(argc, argv)) {
        print_usage();
    }

    fmt::print("Continuing as normal, Ctrl-C to quit.\n");

    auto m = machine<int>{};

    while (1) {
        using namespace std::literals;
        auto input = ""s;
        std::getline(std::cin, input);
        for (const auto &c : input) {
            switch (c) {
            case '+':
                m.increment_data();
                break;
            case '-':
                m.decrement_data();
                break;
            case '>':
                m.next_data();
                break;
            case '<':
                m.previous_data();
                break;
            case '.':
                fmt::print("{}\n", m.output_current());
                break;
            case ',': {
                fmt::print("Data to input:\n>\t");
                auto input = 0;
                std::cin >> input;
                m.input_current(input);
            } break;
            default:
                fmt::print("{} isn't supported\n", c);
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
