/// @file tests/ecs/test_ecs_cxx26.cpp
/// @brief C++26-focused ECS tests for non-default-constructible components.

#include <gba/ecs>
#include <gba/testing>

#include <bit>
#include <type_traits>

struct ndc_comp {
    int value;
    constexpr ndc_comp() = delete;
    constexpr explicit ndc_comp(const int v) : value(v) {}
};

static_assert(!std::is_default_constructible_v<ndc_comp>);
static_assert(std::has_single_bit(sizeof(ndc_comp)));

int main() {
    gba::test("cxx26 ndc emplace and get", [] {
        gba::ecs::registry<8, ndc_comp, int> reg;
        const auto e = reg.create();

        reg.emplace<ndc_comp>(e, 77);
        reg.emplace<int>(e, 5);

        gba::test.expect.eq(reg.get<ndc_comp>(e).value, 77, "non-default component value");
        gba::test.expect.eq(reg.get<int>(e), 5, "second component still works");
    });

    gba::test("cxx26 ndc create_emplace", [] {
        gba::ecs::registry<4, ndc_comp> reg;
        const auto e = reg.create_emplace<ndc_comp>(ndc_comp{13});

        gba::test.expect.eq(reg.get<ndc_comp>(e).value, 13, "create_emplace initializes ndc component");
    });

    gba::test("cxx26 ndc remove and re-emplace", [] {
        gba::ecs::registry<4, ndc_comp> reg;
        const auto e = reg.create();

        reg.emplace<ndc_comp>(e, 1);
        gba::test.expect.eq(reg.get<ndc_comp>(e).value, 1, "first emplace");

        reg.remove<ndc_comp>(e);
        gba::test.expect.is_false(reg.all_of<ndc_comp>(e), "component removed");

        reg.emplace<ndc_comp>(e, 9);
        gba::test.expect.eq(reg.get<ndc_comp>(e).value, 9, "re-emplace after remove");
    });

    return gba::test.finish();
}
