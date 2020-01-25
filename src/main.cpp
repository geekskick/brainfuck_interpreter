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

    fmt::print("Continuing as normal, [Ctrl-CqQ] to quit.\n");

    auto m = machine<int>{};
    auto start_of_loop = std::string::const_iterator{};

    while (1) {
        using namespace std::literals;
        auto input = ""s;
        std::getline(std::cin, input);
        for (auto c = std::cbegin(input); c != std::cend(input);) {
            switch (*c) {
            case '+':
                m.increment_data();
                c++;
                break;
            case '-':
                m.decrement_data();
                c++;
                break;
            case '>':
                m.next_data();
                c++;
                break;
            case '<':
                m.previous_data();
                c++;
                break;
            case '.':
                fmt::print("{}\n", m.output_current());
                c++;
                break;
            case '[':
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
            case ']':
                c = start_of_loop;
                break;
            case ',': {
                fmt::print("Data to input:\n>\t");
                auto input = 0;
                std::cin >> input;
                m.input_current(input);
                c++;
            } break;
            case 'q':
            case 'Q':
                fmt::print("Exitting\n");
                return EXIT_SUCCESS;
                break;
            default:
                fmt::print("{} isn't supported\n", *c);
                break;
            }
        }
    }

    return EXIT_FAILURE;
}
