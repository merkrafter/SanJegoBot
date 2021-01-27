/*
 * Copyright 2021 merkrafter
 *
 * This file is part of libsanjego.
 *
 * libsanjego is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsanjego is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libsanjego. If not, see <https://www.gnu.org/licenses/>.
 */
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
  const RowNr row;
  // column of the tower that should be moved
  const ColumnNr column;
};

class StandardRuleset {
 public:
  /*
   * In this ruleset, a move is allowed if all these conditions hold:
   * - both the source and target position are on the game field
   * - both contain towers
   * - the source tower is owned by the active player
   */
  template <board_size_t HEIGHT, board_size_t WIDTH>
  bool MoveIsAllowedOn(const GameField<HEIGHT, WIDTH> &, const Move &,
                       const Color active_player) const noexcept;

  /*
   * Returns the game-theoretical value of the given game field from the first
   * player's point of view, that is positive values indicate a better position
   * for the first player while negative values are better for the second.
   */
  template <board_size_t HEIGHT, board_size_t WIDTH>
  int8_t ComputeValueOf(const GameField<HEIGHT, WIDTH> &) const noexcept;
};
}  // namespace libsanjego
