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

#include <array>
#include <cstdint>
#include <iterator>
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
bool OwnsTower(Color, Tower);
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
   * Returns the game-theoretical value of the given game board from the first
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
   * Returns the game-theoretical value of the given game board from the first
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
  if (!details::OwnsTower(active_player, sourceTower.value())) {
    return false;
  }
  return true;
}

namespace details {
/*
 * Whether they are legal or not depends on the rule set used.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
auto GetMovesToQuadNeighboursOn(const Board<HEIGHT, WIDTH> &board,
                                const Position pos) noexcept {
  std::array<Move, 4> moves_to_neighbours{
      Move{pos, Position{RowNr(pos.row + 1), pos.column}},
      Move{pos, Position{RowNr(pos.row - 1), pos.column}},
      Move{pos, Position{pos.row, ColumnNr(pos.column + 1)}},
      Move{pos, Position{pos.row, ColumnNr(pos.column - 1)}},
  };

  return moves_to_neighbours;
}
}  // namespace details
template <board_size_t HEIGHT, board_size_t WIDTH>
std::vector<Move> StandardRuleset<HEIGHT, WIDTH>::GetLegalMoves(
    const Board<HEIGHT, WIDTH> &board, const Color active_player) noexcept {
  std::vector<Move> legal_moves;

  for (board_size_t row = 0; row < HEIGHT; ++row) {
    for (board_size_t col = 0; col < WIDTH; ++col) {
      const Position pos{row, col};
      auto moves_to_neighbours =
          details::GetMovesToQuadNeighboursOn(board, pos);
      for (auto &move : moves_to_neighbours) {
        if (MoveIsAllowedOn(board, move, active_player)) {
          legal_moves.push_back(move);
        }
      }
    }
  }

  return legal_moves;
}

typedef std::int8_t game_value_t;

/*
 * Returns the game-theoretical value of the given game board from the first
 * player's point of view, that is positive values indicate a better position
 * for the first player while negative values are better for the second.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
game_value_t StandardRuleset<HEIGHT, WIDTH>::ComputeValueOf(
    const Board<HEIGHT, WIDTH> &board) noexcept {
  return board.height() * board.width() == 1;
}

template <board_size_t HEIGHT, board_size_t WIDTH>
std::unique_ptr<StandardRuleset<HEIGHT, WIDTH>> CreateStandardRulesetFor(
    const Board<HEIGHT, WIDTH> &board) noexcept {
  return std::make_unique<StandardRuleset<HEIGHT, WIDTH>>();
}
}  // namespace libsanjego
