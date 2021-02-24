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
#include <memory>
#include <optional>

#include "libsanjego/gameobjects.hpp"
#include "libsanjego/rulesets.hpp"
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

/*
 * An explorer expands nodes of a game tree to find notable states.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
class Explorer {
 public:
  /*
   * Creates an explorer backed by the standard rule set.
   */
  explicit Explorer()
      : rules(std::make_unique<StandardRuleset<HEIGHT, WIDTH>>()) {}
  virtual SearchResult Explore(const Board<HEIGHT, WIDTH> &board,
                               Color active_player) noexcept = 0;

 protected:
  // TODO allow general rule sets
  std::unique_ptr<StandardRuleset<HEIGHT, WIDTH>> rules;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
class FullExplorer : Explorer<HEIGHT, WIDTH> {
 public:
  SearchResult Explore(const Board<HEIGHT, WIDTH> &board,
                       Color active_player) noexcept;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
SearchResult FullExplorer<HEIGHT, WIDTH>::Explore(
    const Board<HEIGHT, WIDTH> &board, const Color active_player) noexcept {
  if (active_player == Color::Blue) {
    auto result = SearchResult{.num_explored_nodes = 1,
                               .seconds_spent = 0,
                               .best_move = Move{{0, 0}, {0, 1}},
                               .max_explored_depth = 1,
                               .winner = {}};
    if (!this->rules->MoveIsAllowedOn(board, result.best_move, active_player)) {
      result.best_move = Move::Skip();
    }
    return result;
  } else {
    auto result = SearchResult{.num_explored_nodes = 1,
                               .seconds_spent = 0,
                               .best_move = Move{{0, 1}, {0, 0}},
                               .max_explored_depth = 1,
                               .winner = {}};
    if (!this->rules->MoveIsAllowedOn(board, result.best_move, active_player)) {
      result.best_move = Move::Skip();
    }
    return result;
  }
}
}  // namespace libsanjego
