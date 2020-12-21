#include <cstdint>

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
}  // namespace libsanjego
