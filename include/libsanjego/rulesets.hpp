#pragma once

#include <cstdint>

namespace libsanjego {
/*
 * Directions a player can move a tower.
 */
enum class Direction {
  NORTH,
  EAST,
  SOUTH,
  WEST,
};

/*
 * A move a player wants to make in a game.
 * This does not mean a move object is necessarily legal; it depends on the
 * ruleset used.
 */
struct Move {
 public:
  const Direction direction;
  // row of the tower that should be moved
  const uint8_t row;
  // column of the tower that should be moved
  const uint8_t column;
};
}  // namespace libsanjego
