#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "machine.hpp"
#include "states.hpp"

TEST_CASE("Machine") {
    auto m = machine<int>{};

    SECTION("Equality") {
        const auto other = machine<int>{};
        REQUIRE(other == m);
        REQUIRE(m == other);
        SECTION("Data inequality") {
            m.increment_data();
            REQUIRE(m != other);
            REQUIRE(other != m);
        }
        SECTION("Memory length inequality") {
            m.next_data();
            REQUIRE(m != other);
            REQUIRE(other != m);
        }
    }
    SECTION("IO") {
        m.input_current(3);
        const auto uut = m.output_current();
        REQUIRE(uut == 3);
    }

    SECTION("Increments") {
        m.increment_data();
        const auto uut = m.output_current();
        REQUIRE(uut == 1);
    }

    SECTION("Decrements") {
        m.decrement_data();
        const auto uut = m.output_current();
        REQUIRE(uut == -1);
    }

    SECTION("Changes Ptr") {
        m.next_data();
        m.increment_data();
        const auto uut_cell_1 = m.output_current();
        REQUIRE(uut_cell_1 == 1);

        m.previous_data();
        const auto uut_cell_0 = m.output_current();
        REQUIRE(uut_cell_0 == 0);
    }

    SECTION("Zero") { REQUIRE(m.current_is_zero()); }

    SECTION("Current") {
        REQUIRE(m.current_memory_location() == 0);
        m.next_data();
        REQUIRE(m.current_memory_location() == 1);
    }
}

TEST_CASE("States") {
    using namespace std::literals;
    auto m = machine<int>{};
    const auto input = "++[-]"s;

    auto uut = std::unique_ptr<state>{};

    SECTION("Easy next iterators") {
        SECTION("Increment State") {
            uut = std::make_unique<incr_state>();
            uut->perform(m);
            REQUIRE(m.current_memory_location() == 0);
            REQUIRE(m.output_current() == 1);
        }

        SECTION("Decrement State") {
            uut = std::make_unique<decr_state>();
            uut->perform(m);
            REQUIRE(m.current_memory_location() == 0);
            REQUIRE(m.output_current() == -1);
        }

        SECTION("Change Current") {
            uut = std::make_unique<next_state>();
            uut->perform(m);
            REQUIRE(m.output_current() == 0);
            REQUIRE(m.current_memory_location() == 1);

            SECTION("Decrememnt") {
                uut = std::make_unique<prev_state>();
                uut->perform(m);
                REQUIRE(m.output_current() == 0);
                REQUIRE(m.current_memory_location() == 0);
            }
        }
        SECTION("IO") {
            // the io states are tightly coupled to the stdin and stdout at the moment, so only test their next
            // iterators
            SECTION("I") { uut = std::make_unique<input_state>(); }
            SECTION("O") { uut = std::make_unique<output_state>(); }
        }
        // Getting the next instruction should always happen with no change to the machine
        const auto prev_m = m;
        const auto next = uut->next_iterator(input.cbegin(), input, m);
        REQUIRE(std::distance(input.cbegin(), next) == 1);
        REQUIRE(m == prev_m);
    }

    SECTION("Loopy iterators") {
        SECTION("Start of loop") {
            uut = std::make_unique<start_loop_state>();
            SECTION("Skip loop") {
                const auto empty_loop = "[---]"s;
                const auto next = uut->next_iterator(empty_loop.cbegin(), empty_loop, m);
                REQUIRE(empty_loop.cend() == next);

                const auto other_loop = "[-]-"s;
                const auto other_next = uut->next_iterator(other_loop.cbegin(), other_loop, m);
                REQUIRE(3 == std::distance(other_loop.cbegin(), other_next));
            }
            SECTION("Do loop") {
                const auto one_loop = "[-]"s;
                m.increment_data();
                const auto next = uut->next_iterator(one_loop.cbegin(), one_loop, m);
                REQUIRE(*next == '-');
            }
            SECTION("Error") {
                const auto no_loop = "[------------------------------"s;
                const auto next = uut->next_iterator(no_loop.cbegin(), no_loop, m);
                REQUIRE(next == no_loop.cend());
            }
        }
        SECTION("End of loop") {
            uut = std::make_unique<end_loop_state>();
            SECTION("Loops back") {
                const auto command = "[]"s;
                const auto next = uut->next_iterator(command.cbegin() + 1, command, m);
                REQUIRE(command.cbegin() == next);
            }
            SECTION("No start in command") {
                const auto command = "-]-----"s;
                const auto next = uut->next_iterator(command.cbegin() + 1, command, m);
                REQUIRE(next == command.cend());
            }
        }
        // Shouldn't do anything to the machine itself
        const auto prev_m = m;
        uut->perform(m);
        REQUIRE(m == prev_m);
    }
}
