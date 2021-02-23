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
#include <optional>

#include "libsanjego/gameobjects.hpp"
#include "libsanjego/types.hpp"

namespace libsanjego {

/*
 * This struct only supports board sizes up to 255 fields, that is side
 * lengths of 15x17.
 */
struct SearchResult {
  uint64_t num_explored_nodes;
  double seconds_spent;
  Move best_move;
  // counted in half-turns, that is each player's move counts as a level of
  // depth
  uint8_t max_explored_depth;
  // Marks whether there is an enforceable win for a color.
  std::optional<Color> winner;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
class Explorer {
 public:
  virtual SearchResult Explore(const Board<HEIGHT, WIDTH> &board,
                               Color active_player) noexcept = 0;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
class FullExplorer : Explorer<HEIGHT, WIDTH> {
 public:
  SearchResult Explore(const Board<HEIGHT, WIDTH> &board,
                       Color active_player) noexcept;
};
template <board_size_t HEIGHT, board_size_t WIDTH>
SearchResult FullExplorer<HEIGHT, WIDTH>::Explore(
    const Board<HEIGHT, WIDTH> &board, const Color active_player) noexcept {}
}  // namespace libsanjego
