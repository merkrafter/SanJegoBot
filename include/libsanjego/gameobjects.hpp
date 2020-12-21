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

struct Row {
 public:
  constexpr Row(const uint8_t value) noexcept : value(value){};
  constexpr operator uint8_t() const noexcept { return value; }
  uint8_t value;
};

struct Column {
 public:
  constexpr Column(const uint8_t value) noexcept : value(value){};
  constexpr operator uint8_t() const noexcept { return value; }
  uint8_t value;
};

class GameField {
 public:
  /*
   * Creates a new GameField by placing towers of height 1 on the field
   * with the owners alternating in a checkerboard-like pattern.
   */
  GameField(const Row height, const Column width);

  Tower& TowerAt(const Row row, const Column column) noexcept;

  /*
   * Returns a copy of the tower at the given position for read-only tasks.
   */
  Tower GetTowerAt(const Row row, const Column column) const noexcept;

  const Row height;
  const Column width;

 private:
  std::vector<Tower> field;
};
}  // namespace libsanjego
