#include <cstdint>
#include <vector>

#include "catch2/catch.hpp"
#include "libsanjego/gameobjects.hpp"
#include "libsanjego/rulesets.hpp"

// To make the test cases more readable
using namespace libsanjego;

TEST_CASE("Basic move is allowed", "[fast]") {
  const Board<2, 2> board;
  const StandardRuleset ruleset;

  const Move move = {Direction::EAST, 0, 0};
  REQUIRE(ruleset.MoveIsAllowedOn(board, move, Color::BLUE));
}

TEST_CASE("On a 1x1 board, any move is illegal", "[fast]") {
  const Board<1, 1> board;
  const StandardRuleset ruleset;

  Move move{Direction::NORTH, 0, 0};
  REQUIRE_FALSE(ruleset.MoveIsAllowedOn(board, move, Color::BLUE));
}

TEST_CASE("Moves over border are illegal", "[fast]") {
  const Board<2, 2> board;
  const StandardRuleset ruleset;

  std::vector<Move> moves{
      Move{Direction::NORTH, 0, 0},
      Move{Direction::WEST, 0, 0},
      Move{Direction::EAST, 1, 1},
      Move{Direction::SOUTH, 1, 1},
  };

  for (int i = 0; i < moves.size(); i++) {
    const auto move = moves[i];
    REQUIRE_FALSE(ruleset.MoveIsAllowedOn(board, move, Color::BLUE));
  }
}

TEST_CASE("Moving a tower is illegal if a player does not own it", "[fast]") {
  const Board<2, 2> board;
  const StandardRuleset ruleset;

  SECTION("First player moves second player's tower") {
    const Move move{Direction::WEST, 0, 1};
    REQUIRE_FALSE(ruleset.MoveIsAllowedOn(board, move, Color::BLUE));
  }

  SECTION("Second player moves first player's tower") {
    const Move move{Direction::EAST, 0, 0};
    REQUIRE_FALSE(ruleset.MoveIsAllowedOn(board, move, Color::Yellow));
  }
}
// 3. move not allowed if no target tower
// 4. move not allowed if no source tower

// In this scenario, there is only one tower on the board which belongs to
// the first player.
TEST_CASE("Newly created Board of size 1 should have value 1", "[fast]") {
  const Board<1, 1> board;
  const StandardRuleset ruleset;
  REQUIRE(ruleset.ComputeValueOf(board) == 1);
}

TEST_CASE("Newly created Board should have value 0", "[fast]") {
  const Board<2, 2> board;
  const StandardRuleset ruleset;
  REQUIRE(ruleset.ComputeValueOf(board) == 0);
}
