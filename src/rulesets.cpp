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
#include "rulesets.hpp"

namespace libsanjego {

template <board_size_t HEIGHT, board_size_t WIDTH>
inline bool exceeds_border(const Board<HEIGHT, WIDTH>& field,
                           const Move& move) {
  return move.row == 0 && move.direction == Direction::NORTH ||
         move.column == 0 && move.direction == Direction::WEST ||
         move.row + 1 >= field.height && move.direction == Direction::SOUTH ||
         move.column + 1 >= field.width && move.direction == Direction::EAST;
}

inline bool owns_tower(const Color active_player, const Tower tower) {
  return tower.Top() == active_player;
}

template <board_size_t HEIGHT, board_size_t WIDTH>
bool StandardRuleset::MoveIsAllowedOn(
    const Board<HEIGHT, WIDTH>& field, const Move& move,
    const Color active_player) const noexcept {
  if (exceeds_border(field, move)) {
    return false;
  }
  if (!owns_tower(active_player, field.GetTowerAt(move.row, move.column))) {
    return false;
  }
  return true;
}
template <board_size_t HEIGHT, board_size_t WIDTH>
int8_t StandardRuleset::ComputeValueOf(
    const Board<HEIGHT, WIDTH>& field) const noexcept {
  return field.height * field.width == 1;
}
}  // namespace libsanjego
