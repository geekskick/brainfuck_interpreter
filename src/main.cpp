#include "fmt/format.h"
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

    fmt::print("Continuing as normal\n");
    return EXIT_SUCCESS;
}
