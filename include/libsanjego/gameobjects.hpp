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

// Type that represents the size of a game field.
typedef uint8_t gf_size_t;

struct Row {
 public:
  constexpr Row(const gf_size_t value) noexcept : value(value){};
  constexpr operator gf_size_t() const noexcept { return value; }
  gf_size_t value;
};

struct Column {
 public:
  constexpr Column(const gf_size_t value) noexcept : value(value){};
  constexpr operator gf_size_t() const noexcept { return value; }
  gf_size_t value;
};

class GameField {
 public:
  /*
   * Creates a new GameField by placing towers of height 1 on the field
   * with the owners alternating in a checkerboard-like pattern.
   */
  GameField(const Row height, const Column width);

  Tower &TowerAt(const Row row, const Column column) noexcept;

  /*
   * Returns a copy of the tower at the given position for read-only tasks.
   */
  Tower GetTowerAt(const Row row, const Column column) const noexcept;

  const Row height;
  const Column width;

 private:
  std::vector<Tower> field;
};

/*
 * Factory function for GameFields that chooses the most efficient
 * implementation for the given width and height.
 */
template <gf_size_t HEIGHT, gf_size_t WIDTH>
GameField CreateGameField();
}  // namespace libsanjego
