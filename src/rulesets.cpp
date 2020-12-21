#include "rulesets.hpp"

namespace libsanjego {

inline bool exceeds_border(const GameField& field, const Move& move) {
  return move.row == 0 && move.direction == Direction::NORTH ||
         move.column == 0 && move.direction == Direction::WEST ||
         move.row + 1 >= field.height && move.direction == Direction::SOUTH ||
         move.column + 1 >= field.width && move.direction == Direction::EAST;
}

inline bool owns_tower(const Player active_player, const Tower tower) {
  return tower.Owner() == active_player;
}

bool StandardRuleset::MoveIsAllowedOn(
    const GameField& field, const Move& move,
    const Player active_player) const noexcept {
  if (exceeds_border(field, move)) {
    return false;
  }
  if (!owns_tower(active_player, field.GetTowerAt(move.row, move.column))) {
    return false;
  }
  return true;
}
int8_t StandardRuleset::ComputeValueOf(const GameField& field) const noexcept {
  return field.height * field.width == 1;
}
}  // namespace libsanjego
