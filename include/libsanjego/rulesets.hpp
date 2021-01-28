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
 * A move a player wants to make in a game.
 * This does not mean a move object is necessarily legal; it depends on the
 * ruleset used.
 */
struct Move {
  const Position source;
  const Position target;
};

namespace details {

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
  const std::optional<Tower> sourceTower = board.GetTowerAt(move.source);
  if (!sourceTower.has_value()) {
    return false;
  }
  const std::optional<Tower> targetTower = board.GetTowerAt(move.target);
  if (!targetTower.has_value()) {
    return false;
  }
  if (!details::owns_tower(active_player, sourceTower.value())) {
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
  return board.Height() * board.Width() == 1;
}
}  // namespace libsanjego
