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

}  // namespace libsanjego
