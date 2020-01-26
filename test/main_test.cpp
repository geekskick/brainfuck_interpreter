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
    const auto input = "hello"s;

    SECTION("Easy next iterators") {
        auto uut = std::unique_ptr<state>{};
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
        SECTION("IO"){
            // the io states are tightly coupled to the stdin and stdout at the moment, so only test their next iterators
           SECTION("I"){
                uut = std::make_unique<input_state>();
           } 
           SECTION("O"){
                uut = std::make_unique<output_state>();
           }
        }
        const auto prev_m = m;
        const auto next = uut->next_iterator(input.cbegin(), input, m);
        REQUIRE(std::distance(input.cbegin(), next) == 1);
        REQUIRE(m == prev_m);
    }
}
