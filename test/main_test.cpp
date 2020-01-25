#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "machine.hpp"

TEST_CASE("Input and Output") {
    auto m = machine<int>{};
    m.input_current(3);
    const auto uut = m.output_current();
    REQUIRE(uut == 3);
}

TEST_CASE("Increments") {
    auto m = machine<int>{};

    m.increment_data();
    const auto uut = m.output_current();
    REQUIRE(uut == 1);
}

TEST_CASE("Decrements") {
    auto m = machine<int>{};

    m.decrement_data();
    const auto uut = m.output_current();
    REQUIRE(uut == -1);
}

TEST_CASE("Changes Ptr") {
    auto m = machine<int>{};

    m.next_data();
    m.increment_data();
    const auto uut_cell_1 = m.output_current();
    REQUIRE(uut_cell_1 == 1);

    m.previous_data();
    const auto uut_cell_0 = m.output_current();
    REQUIRE(uut_cell_0 == 0);
}

TEST_CASE("Changes Ptr Without changing data") {
    auto m = machine<int>{};

    m.next_data();
    m.increment_data();
    const auto uut_cell_1 = m.output_current();
    REQUIRE(uut_cell_1 == 1);

    m.previous_data();
    const auto uut_cell_0 = m.output_current();
    REQUIRE(uut_cell_0 == 0);

    m.next_data();
    const auto uut_cell_1_again = m.output_current();
    REQUIRE(uut_cell_1_again == uut_cell_1);
}
TEST_CASE("Zero"){
    auto m = machine<int>{};
    REQUIRE(m.current_is_zero());
}
