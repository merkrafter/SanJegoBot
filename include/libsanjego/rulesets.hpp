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
  const Position from;
  const Direction direction;
};

namespace details {
template <board_size_t HEIGHT, board_size_t WIDTH>
bool exceeds_border(const Board<HEIGHT, WIDTH> &board, const Move &move) {
  return move.from.row == 0 && move.direction == Direction::NORTH ||
         move.from.column == 0 && move.direction == Direction::WEST ||
         move.from.row + 1 >= board.height &&
             move.direction == Direction::SOUTH ||
         move.from.column + 1 >= board.width &&
             move.direction == Direction::EAST;
}

bool owns_tower(Color, Tower);
}  // namespace details

/*
 * In this ruleset, a move is allowed if all these conditions hold:
 * - both the source and target position are on the board
 * - both contain towers
 * - the source tower is owned by the active player
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
bool MoveIsAllowedOn(const Board<HEIGHT, WIDTH> &board, const Move &move,
                     const Color active_player) noexcept {
  if (details::exceeds_border<HEIGHT, WIDTH>(board, move)) {
    return false;
  }
  const std::optional<Tower> sourceTower = board.GetTowerAt(move.from);
  if (sourceTower.has_value() &&
      !details::owns_tower(active_player, sourceTower.value())) {
    return false;
  }
  return true;
}

/*
 * Returns the game-theoretical value of the given game field from the first
 * player's point of view, that is positive values indicate a better position
 * for the first player while negative values are better for the second.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
int8_t ComputeValueOf(const Board<HEIGHT, WIDTH> &board) noexcept {
  return board.height * board.width == 1;
}
}  // namespace libsanjego
