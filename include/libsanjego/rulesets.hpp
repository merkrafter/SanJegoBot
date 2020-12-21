#pragma once

#include <cstdint>

#include "gameobjects.hpp"

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

class StandardRuleset {
 public:
  /*
   * In this ruleset, a move is allowed if all these conditions hold:
   * - both the source and target position are on the game field
   * - both contain towers
   * - the source tower is owned by the active player
   */
  bool MoveIsAllowedOn(const GameField&, const Move&,
                       const Player active_player) const noexcept;

  /*
   * Returns the gametheoretical value of the given game field from the first
   * player's point of view, that is positive values indicate a better position
   * for the first player while negative values are better for the second.
   */
  int8_t ComputeValueOf(const GameField&) const noexcept;
};
}  // namespace libsanjego
