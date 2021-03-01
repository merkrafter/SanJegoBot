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

#include <algorithm>
#include <cstdint>
#include <optional>
#include <utility>
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
  explicit Tower(Color color, tower_size_t height);
  [[nodiscard]] Color top() const noexcept;
  [[nodiscard]] tower_size_t height() const noexcept;

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
  tower_size_t representation_;
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
uint32_t ToArrayIndex(Position position, ColumnNr board_width);

/*
 * Initializes the field with height*width towers.
 * Assumes the field is empty before the operation.
 */
void SetCheckerboardPattern(std::vector<Tower> &field, RowNr height,
                            ColumnNr width);

/*
 * Returns whether the given position lies outside the rectangular region
 * defined by the template parameters and (0,0).
 */
template <board_size_t HEIGHT, board_size_t WIDTH>
bool ExceedsBorder(const Position &position) {
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

namespace details {
// function to make the query more readable
inline bool WasMadeBefore(const Move &move) noexcept {
  return move.affected_tower.has_value();
}
}  // namespace details

template <board_size_t HEIGHT, board_size_t WIDTH>
class Board {
 public:
  /*
   * Creates a new Board by placing towers of height 1 on the field
   * with the colors alternating in a checkerboard-like pattern.
   */
  Board() {
    fields_.reserve(HEIGHT * WIDTH);
    details::SetCheckerboardPattern(fields_, HEIGHT, WIDTH);
  }

  /*
   * Makes the move on this board. It does not check whether the move is
   * actually legal (apart from bound checks).
   * Returns whether the move was carried out successfully.
   */
  bool Make(Move &move) noexcept {
    if (details::ExceedsBorder<HEIGHT, WIDTH>(move.source) ||
        details::ExceedsBorder<HEIGHT, WIDTH>(move.target) ||
        move.source == move.target) {
      return false;
    }
    auto &source_tower =
        this->fields_[details::ToArrayIndex(move.source, width())];
    auto &target_tower =
        this->fields_[details::ToArrayIndex(move.target, width())];
    move.affected_tower = target_tower;
    target_tower.Attach(source_tower);
    source_tower.Clear();
    return true;
  }

  /*
   * Reverts a move previously made on this board and returns whether this
   * was successful.
   * This operation fails is the coordinates of the move are outside the board's
   * borders, the move has no information about the state before it was made,
   * or the source position of the move is not empty, indicating that the move
   * has not been done on *this* board before.
   */
  bool Undo(Move &move) noexcept {
    if (details::ExceedsBorder<HEIGHT, WIDTH>(move.source) ||
        details::ExceedsBorder<HEIGHT, WIDTH>(move.target) ||
        move.source == move.target) {
      return false;
    }

    if (not details::WasMadeBefore(move)) {
      return false;
    }

    auto &source_tower =
        this->fields_[details::ToArrayIndex(move.source, width())];
    if (not source_tower.IsEmpty()) {
      return false;
    }
    auto &target_tower =
        this->fields_[details::ToArrayIndex(move.target, width())];

    std::swap(source_tower, target_tower);
    target_tower = move.affected_tower.value();
    source_tower.DetachFrom(target_tower);

    return true;
  }

  /*
   * Returns a copy of the tower at the given position for read-only tasks if
   * the board has a tower at the specified position.
   */
  [[nodiscard]] std::optional<Tower> GetTowerAt(
      Position position) const noexcept {
    if (details::ExceedsBorder<HEIGHT, WIDTH>(position)) {
      return {};
    }
    const auto tower = this->fields_[details::ToArrayIndex(position, width())];
    if (tower.IsEmpty()) {
      return {};
    }
    return tower;
  }

  /*
   * Computes the height of the highest tower that is owned by the player with
   * the given color.
   * If there is no tower with the given owner on the board, 0 is returned.
   */
  [[nodiscard]] tower_size_t MaxHeightOf(const Color owner) const noexcept {
    tower_size_t max_height = 0;
    for (const Tower &tower : fields_) {
      if (tower.top() == owner) {
        max_height = std::max(max_height, tower.height());
      }
    }
    return max_height;
  }

  [[nodiscard]] constexpr RowNr height() const noexcept { return HEIGHT; }
  [[nodiscard]] constexpr ColumnNr width() const noexcept { return WIDTH; }

 private:
  std::vector<Tower> fields_;
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
