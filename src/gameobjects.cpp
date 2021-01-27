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
#include "gameobjects.hpp"

#include "types.hpp"

namespace libsanjego {

// least significant bit
constexpr uint8_t OWNER_BIT = 1;
constexpr uint8_t without_owner(uint8_t data) { return data >> OWNER_BIT; }

Tower::Tower(const Player owner)
    : representation(0b10 | static_cast<uint8_t>(owner)) {}

Player Tower::Owner() const noexcept {
  return static_cast<Player>(this->representation & OWNER_BIT);
}

tower_size_t Tower::Height() const noexcept {
  return without_owner(this->representation);
}

constexpr uint32_t dual_to_single_index(const RowNr row, const ColumnNr column,
                                        const RowNr height) {
  return row * height + column;
}

void set_checkerboard_pattern(std::vector<Tower> field, const RowNr height,
                              const ColumnNr width) {
  field.reserve(height * width);
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      const auto idx = dual_to_single_index(row, col, height);
      this->field[idx] = Tower(static_cast<Player>((row + col) % 2));
    }
  }
}

template <board_size_t HEIGHT, board_size_t WIDTH>
Tower& GameField<HEIGHT, WIDTH>::TowerAt(const RowNr row,
                                         const ColumnNr column) noexcept {
  return this->field[dual_to_single_index(row, column, this->height)];
}

template <board_size_t HEIGHT, board_size_t WIDTH>
Tower GameField<HEIGHT, WIDTH>::GetTowerAt(const Row row,
                                           const Column column) const noexcept {
  return this->field[dual_to_single_index(row, column, this->height)];
}
}  // namespace libsanjego
