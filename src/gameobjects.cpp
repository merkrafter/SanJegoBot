#include "gameobjects.hpp"

namespace libsanjego {

// least significant bit
constexpr uint8_t OWNER_BIT = 1;
constexpr uint8_t without_owner(uint8_t data) { return data >> OWNER_BIT; }

Tower::Tower(const Player owner)
    : representation(0b10 | static_cast<uint8_t>(owner)) {}

Player Tower::Owner() const noexcept {
  return static_cast<Player>(this->representation & OWNER_BIT);
}

uint8_t Tower::Height() const noexcept {
  return without_owner(this->representation);
}

constexpr uint32_t dual_to_single_index(const uint8_t row, const uint8_t column,
                                        const uint8_t height) {
  return row * height + column;
}

GameField::GameField(const uint8_t height, const uint8_t width)
    : height(height), width(width) {
  this->field.reserve(height * width);
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      const auto idx = dual_to_single_index(row, col, height);
      this->field[idx] = Tower(static_cast<Player>((row + col) % 2));
    }
  }
}

Tower& GameField::TowerAt(const uint8_t row, const uint8_t column) noexcept {
  return this->field[dual_to_single_index(row, column, this->height)];
}

Tower GameField::GetTowerAt(const uint8_t row,
                            const uint8_t column) const noexcept {
  return this->field[dual_to_single_index(row, column, this->height)];
}
}  // namespace libsanjego
