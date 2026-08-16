/// @file bits/ecs/entity.hpp
/// @brief Entity identifier and ECS utility types.
#pragma once

#include <cstddef>
#include <cstdint>

namespace gba {

    /// @brief Entity handle: slot + generation packed in uint16_t.
    ///
    /// Slot identifies the storage index. Generation detects use-after-destroy.
    /// Default-constructed entity is null (slot=0xFF, generation=0xFF).
    /// Maximum 255 live entities (slots 0-254); slot 255 is reserved for null.
    ///
    /// Treat as a value type; pass by `const entity` when making semantic immutability clear.
    struct alignas(std::uint16_t) entity {
        std::uint8_t slot;
        std::uint8_t generation;

        /// Default constructor: produces null entity.
        constexpr entity() noexcept : slot(0xFF), generation(0xFF) {}

        /// Construct from slot and generation.
        constexpr entity(std::uint8_t s, std::uint8_t g) noexcept : slot(s), generation(g) {}

        constexpr bool operator==(const entity&) const = default;
        constexpr bool operator!=(const entity&) const = default;
    };

    /// @brief Sentinel entity handle: default-constructed entity (null by design).
    inline constexpr entity entity_null;

} // namespace gba
