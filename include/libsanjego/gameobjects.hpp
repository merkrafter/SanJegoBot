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
enum struct Color : uint8_t { BLUE = 0, Yellow = 1 };

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

 private:
  tower_size_t representation;
};

struct Position {
  RowNr row;
  ColumnNr column;
};

namespace details {
uint32_t to_array_index(Position position, ColumnNr boardWidth);

void set_checkerboard_pattern(std::vector<Tower> field, RowNr height,
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

template <board_size_t HEIGHT, board_size_t WIDTH>
class Board {
 public:
  /*
   * Creates a new Board by placing towers of height 1 on the field
   * with the colors alternating in a checkerboard-like pattern.
   */
  Board() { details::set_checkerboard_pattern(field, HEIGHT, WIDTH); }

  /*
   * Returns a copy of the tower at the given position for read-only tasks if
   * the board has a tower at the specified position.
   */
  [[nodiscard]] std::optional<Tower> GetTowerAt(
      Position position) const noexcept {
    if (details::exceeds_border<HEIGHT, WIDTH>(position)) {
      return {};
    }
    return this->field[details::to_array_index(position, Width())];
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
