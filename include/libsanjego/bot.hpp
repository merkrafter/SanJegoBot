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

#include <chrono>
#include <cstdint>
#include <limits>
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

/*
 * Searches the game tree to find interesting states and strong moves.
 * Returns a move considered "best" as well as some statistics on the search.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
SearchResult FullExplorer<HEIGHT, WIDTH>::Explore(
    const Board<HEIGHT, WIDTH> &board, const Color active_player) noexcept {
  const auto start = std::chrono::steady_clock::now();
  auto possible_moves = this->rules->GetLegalMoves(board, active_player);
  auto best_move = Move::Skip();
  if (!possible_moves.empty()) {
    auto max_rating = std::numeric_limits<game_value_t>::min();
    auto tmp_board(board);
    for (auto &move : possible_moves) {
      tmp_board.Make(move);
      const auto rating = this->rules->ComputeValueOf(tmp_board);
      tmp_board.Undo(move);
      if (rating > max_rating) {
        max_rating = rating;
        best_move = move;
      }
    }
  }
  const auto end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> seconds_spent = end - start;
  return SearchResult{
      .num_explored_nodes = possible_moves.size(),
      .seconds_spent = seconds_spent.count(),
      .best_move = best_move,
      .max_explored_depth = 1,
      .winner = {},
  };
}
}  // namespace libsanjego
