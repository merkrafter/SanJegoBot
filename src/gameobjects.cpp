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
constexpr tower_size_t pack(const tower_size_t height, const Color color) {
  return height << OWNER_BIT | static_cast<uint8_t>(color);
}

Tower::Tower(const Color color) : representation(pack(1, color)) {}

Color Tower::Top() const noexcept {
  return static_cast<Color>(this->representation & OWNER_BIT);
}

tower_size_t Tower::Height() const noexcept {
  return without_owner(this->representation);
}

bool Tower::IsEmpty() const noexcept { return this->representation == 0; }

void Tower::Attach(const Tower tower) {
  if (tower.IsEmpty()) {
    return;
  }
  const auto new_height = this->Height() + tower.Height();
  const auto new_owner = tower.Top();
  this->representation = pack(new_height, new_owner);
}

bool Position::operator==(const Position &other) const noexcept {
  return this->row == other.row && this->column == other.column;
}

namespace details {
uint32_t to_array_index(const Position position, const ColumnNr boardWidth) {
  return position.row * boardWidth + position.column;
}

void set_checkerboard_pattern(std::vector<Tower> &field, const RowNr height,
                              const ColumnNr width) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      field.push_back(Tower(static_cast<Color>((row + col) % 2)));
    }
  }
}
}  // namespace details

}  // namespace libsanjego
