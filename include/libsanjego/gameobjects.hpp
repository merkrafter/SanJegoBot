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
#include <vector>

#include "types.hpp"

namespace libsanjego {
enum struct Color : uint8_t { Blue = 0, Yellow = 1 };

// Forward declaration for use in Tower class.
template <board_size_t HEIGHT, board_size_t WIDTH>
class Board;

/*
 * This Tower implementation does not preserve the actual structure,
 * that is the order of bricks it consists of. It only stores its height and
 * the owner in an internal structure like this:
 * [height: bit 7..1 | owner: bit 0]
 * which saves a lot of space compared to an structure preserving
 * implementation.
 */
class Tower {
 public:
  explicit Tower(Color color);
  [[nodiscard]] Color Top() const noexcept;
  [[nodiscard]] tower_size_t Height() const noexcept;

  /*
   * Adds the given tower on top of *this* one. As a result, their heights are
   * added and the top brick is now the same as tower's.
   *
   * Example:
   *   ```cpp
   *   // initial situation (ground is left)
   *   // source: |BBY
   *   // target: |B
   *
   *   target.Attach(source)
   *
   *   // source: |BBY
   *   // target: |BBBY
   *   ```
   */
  void Attach(const Tower tower);

  /*
   * Removes the given tower from the bottom of *this* one. It is the inverse
   * operation of Attach (note that the arguments are also inverted).
   * As a result, *this* tower is shortened by the height of the given tower,
   * and its top brick is left untouched. If the given tower does not form the
   * bottom of *this* tower (leaving at least one brick in *this* instance), the
   * operation does nothing.
   *
   * Example:
   *   ```cpp
   *   // initial situation (ground is left)
   *   // source: |BBBY
   *   // target: |B
   *
   *   source.DetachFrom(target)
   *
   *   // source: |BBY
   *   // target: |B
   *   ```
   */
  void DetachFrom(const Tower tower);

  template <board_size_t HEIGHT, board_size_t WIDTH>
  friend class Board;

 private:
  tower_size_t representation;
  /*
   * Marks this tower instance as empty. This is a safe alternative to nullptr.
   */
  void Clear() noexcept;
  /*
   * Returns whether this tower is actually a null value.
   */
  [[nodiscard]] bool IsEmpty() const noexcept;
};

struct Position {
  RowNr row;
  ColumnNr column;

  bool operator==(const Position &other) const noexcept;
};

namespace details {
uint32_t to_array_index(Position position, ColumnNr boardWidth);

/*
 * Initializes the field with height*width towers.
 * Assumes the field is empty before the operation.
 */
void set_checkerboard_pattern(std::vector<Tower> &field, RowNr height,
                              ColumnNr width);

/*
 * Returns whether the given position lies outside the rectangular region
 * defined by the template parameters and (0,0).
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
bool exceeds_border(const Position &position) {
  return position.row < 0 || position.column < 0 || position.row >= HEIGHT ||
         position.column >= WIDTH;
}
}  // namespace details

/*
 * A move a player wants to make in a game.
 * This does not mean a move object is necessarily legal; it depends on the
 * ruleset used.
 * If a move is applied to a board, the target tower may be stored in this
 * struct's affected_tower field to simplify reverting moves.
 */
struct Move {
  Position source;
  Position target;
  std::optional<Tower> affected_tower;

  bool operator==(const Move &other) const noexcept;
  Move &operator=(const Move &other) = default;
  [[nodiscard]] bool IsSkip() const noexcept;

  /*
   * Creates a move that signals that the player wants to skip the turn.
   */
  static Move Skip() noexcept;
};

template <board_size_t HEIGHT, board_size_t WIDTH>
class Board {
 public:
  /*
   * Creates a new Board by placing towers of height 1 on the field
   * with the colors alternating in a checkerboard-like pattern.
   */
  Board() {
    field.reserve(HEIGHT * WIDTH);
    details::set_checkerboard_pattern(field, HEIGHT, WIDTH);
  }

  /*
   * Makes the move on this board. It does not check whether the move is
   * actually legal (apart from bound checks).
   * Returns whether the move was carried out successfully.
   */
  bool Make(Move &move) noexcept {
    if (details::exceeds_border<HEIGHT, WIDTH>(move.source) ||
        details::exceeds_border<HEIGHT, WIDTH>(move.target) ||
        move.source == move.target) {
      return false;
    }
    auto &sourceTower =
        this->field[details::to_array_index(move.source, Width())];
    auto &targetTower =
        this->field[details::to_array_index(move.target, Width())];
    move.affected_tower = targetTower;
    targetTower.Attach(sourceTower);
    sourceTower.Clear();
    return true;
  }

  /*
   * Returns a copy of the tower at the given position for read-only tasks if
   * the board has a tower at the specified position.
   */
  [[nodiscard]] std::optional<Tower> GetTowerAt(
      Position position) const noexcept {
    if (details::exceeds_border<HEIGHT, WIDTH>(position)) {
      return {};
    }
    const auto tower = this->field[details::to_array_index(position, Width())];
    if (tower.IsEmpty()) {
      return {};
    }
    return tower;
  }

  [[nodiscard]] constexpr RowNr Height() const noexcept { return HEIGHT; }
  [[nodiscard]] constexpr ColumnNr Width() const noexcept { return WIDTH; }

 private:
  std::vector<Tower> field;
};

/*
 * Factory function for Boards that chooses the most efficient
 * implementation for the given width and height.
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
Board<HEIGHT, WIDTH> CreateBoard() {
  return Board<HEIGHT, WIDTH>();
}
}  // namespace libsanjego
