#pragma once

#include <cstdint>
#include <vector>

namespace libsanjego {
enum struct Player : uint8_t { FIRST = 0, SECOND = 1 };

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
  Tower(const Player owner);
  Player Owner() const noexcept;
  uint8_t Height() const noexcept;

 private:
  uint8_t representation;
};

class GameField {
 public:
  /*
   * Creates a new GameField by placing towers of height 1 on the field
   * with the owners alternating in a checkerboard-like pattern.
   */
  GameField(const uint8_t height, const uint8_t width);

  Tower& TowerAt(const uint8_t row, const uint8_t column) noexcept;

  /*
   * Returns a copy of the tower at the given position for read-only tasks.
   */
  Tower GetTowerAt(const uint8_t row, const uint8_t column) const noexcept;

  const uint8_t height;
  const uint8_t width;

 private:
  std::vector<Tower> field;
};
}  // namespace libsanjego
