#include <reaction.hpp>
#include <doctest/doctest.h>
#include <string>

TEST_CASE("Reaction pretty print") {
    Reaction<std::string, float> r("A", 1.0f);
    r.lambda = 0.5;
    r.input = {"A"};
    r.product = {"B"};
    CHECK_NOTHROW(r.print_reaction());
}

TEST_CASE("Reaction operator overloads") {
    Reaction<std::string, float> A("A", 1.0f);
    Reaction<std::string, float> B("B", 2.0f);

    auto r1 = (A + B);
    CHECK(r1.first.get_key() == "A");
    CHECK(r1.second.get_key() == "B");

    auto r2 = (A >> 0.1f);
    CHECK(r2.lambda == doctest::Approx(0.1f));

    auto r3 = (A >> 0.2f >>= B);
    CHECK(r3.lambda == doctest::Approx(0.2f));
    CHECK(r3.product[0] == "B");
}
