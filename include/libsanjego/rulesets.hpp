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
#include <vector>

#include "gameobjects.hpp"

namespace libsanjego {
namespace details {

/*
 * Returns whether the player with the given color is allowed to move the given
 * tower.
 * In the standard ruleset, this is the case if the top brick of the tower has
 * the given color.
 */
bool owns_tower(Color, Tower);
}  // namespace details

/*
 * A ruleset brings life into game objects and defines how they are allowed to
 * interact with each other.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
class Ruleset {
 public:
  virtual bool MoveIsAllowedOn(const Board<HEIGHT, WIDTH> &board,
                               const Move &move,
                               const Color active_player) noexcept = 0;
  /*
   * Returns the game-theoretical value of the given game field from the first
   * player's point of view, that is positive values indicate a better position
   * for the first player while negative values are better for the second.
   */
  virtual int8_t ComputeValueOf(const Board<HEIGHT, WIDTH> &board) noexcept = 0;

  virtual std::vector<Move> GetLegalMoves(
      const Board<HEIGHT, WIDTH> &board,
      const Color active_player) noexcept = 0;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
class StandardRuleset : Ruleset<HEIGHT, WIDTH> {
 public:
  /*
   * In this ruleset, a move is allowed if all these conditions hold:
   * - both the source and target are distinct positions on the board
   * - both contain towers
   * - the source tower is owned by the active player
   */
  bool MoveIsAllowedOn(const Board<HEIGHT, WIDTH> &board, const Move &move,
                       const Color active_player) noexcept;
  /*
   * Returns the game-theoretical value of the given game field from the first
   * player's point of view, that is positive values indicate a better position
   * for the first player while negative values are better for the second.
   *
   * In this ruleset, the value is the difference in height of the highest
   * tower of each player.
   */
  int8_t ComputeValueOf(const Board<HEIGHT, WIDTH> &board) noexcept;

  std::vector<Move> GetLegalMoves(const Board<HEIGHT, WIDTH> &board,
                                  const Color active_player) noexcept;
};

/*
 * In this ruleset, a move is allowed if all these conditions hold:
 * - both the source and target are distinct positions on the board
 * - both contain towers
 * - the source tower is owned by the active player
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
bool StandardRuleset<HEIGHT, WIDTH>::MoveIsAllowedOn(
    const Board<HEIGHT, WIDTH> &board, const Move &move,
    const Color active_player) noexcept {
  if (move.source == move.target) {
    return false;
  }

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

template <board_size_t HEIGHT, board_size_t WIDTH>
std::vector<Move> StandardRuleset<HEIGHT, WIDTH>::GetLegalMoves(
    const Board<HEIGHT, WIDTH> &board, const Color active_player) noexcept {
  std::vector<Move> legal_moves;

  return legal_moves;
}

/*
 * Returns the game-theoretical value of the given game field from the first
 * player's point of view, that is positive values indicate a better position
 * for the first player while negative values are better for the second.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
int8_t StandardRuleset<HEIGHT, WIDTH>::ComputeValueOf(
    const Board<HEIGHT, WIDTH> &board) noexcept {
  return board.Height() * board.Width() == 1;
}

template <board_size_t HEIGHT, board_size_t WIDTH>
std::unique_ptr<StandardRuleset<HEIGHT, WIDTH>> CreateStandardRulesetFor(
    const Board<HEIGHT, WIDTH> &board) noexcept {
  return std::make_unique<StandardRuleset<HEIGHT, WIDTH>>();
}

}  // namespace libsanjego
