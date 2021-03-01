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
constexpr uint8_t WithoutOwner(uint8_t data) { return data >> OWNER_BIT; }
constexpr tower_size_t Pack(const tower_size_t height, const Color color) {
  return height << OWNER_BIT | static_cast<uint8_t>(color);
}

Tower::Tower(const Color color) : representation_(Pack(1, color)) {}

Tower::Tower(Color color, tower_size_t height)
    : representation_(Pack(height, color)) {}

Color Tower::top() const noexcept {
  return static_cast<Color>(this->representation_ & OWNER_BIT);
}

tower_size_t Tower::height() const noexcept {
  return WithoutOwner(this->representation_);
}

void Tower::Clear() noexcept { this->representation_ = 0; }

bool Tower::IsEmpty() const noexcept { return this->representation_ == 0; }

void Tower::Attach(const Tower tower) {
  if (tower.IsEmpty()) {
    return;
  }
  const auto new_height = this->height() + tower.height();
  const auto new_owner = tower.top();
  this->representation_ = Pack(new_height, new_owner);
}

void Tower::DetachFrom(const Tower tower) {
  const auto this_height = this->height();
  const auto other_height = tower.height();
  if (other_height >= this_height) {
    return;
  }
  const auto new_height = this_height - other_height;
  const auto new_owner = this->top();
  this->representation_ = Pack(new_height, new_owner);
}

bool Move::operator==(const Move &other) const noexcept {
  return this->source == other.source && this->target == other.target;
}

Move Move::Skip() noexcept { return Move{0, 0, 0, 0}; }

bool Move::IsSkip() const noexcept {
  return this->source.column == 0 && this->source.row == 0 &&
         this->target.column == 0 && this->target.row == 0;
}

bool Position::operator==(const Position &other) const noexcept {
  return this->row == other.row && this->column == other.column;
}

namespace details {
uint32_t ToArrayIndex(Position position, ColumnNr board_width) {
  return position.row * board_width + position.column;
}

void SetCheckerboardPattern(std::vector<Tower> &field, RowNr height,
                            ColumnNr width) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      field.push_back(Tower(static_cast<Color>((row + col) % 2)));
    }
  }
}
}  // namespace details
}  // namespace libsanjego
