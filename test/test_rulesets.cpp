#include <cstdint>
#include <vector>

#include "catch2/catch.hpp"
#include "libsanjego/gameobjects.hpp"
#include "libsanjego/rulesets.hpp"

// To make the test cases more readable
using namespace libsanjego;

TEST_CASE("Basic move is allowed", "[fast]") {
  const Board<2, 2> board;

  const Move move = {0, 0, 0, 1};
  REQUIRE(MoveIsAllowedOn<2, 2>(board, move, Color::Blue));
}

TEST_CASE("On a 1x1 board, any move is illegal", "[fast]") {
  const Board<1, 1> board;

  Move move{0, 0, 0, 1};
  REQUIRE_FALSE(MoveIsAllowedOn<1, 1>(board, move, Color::Blue));
}

TEST_CASE("Moves over border are illegal", "[fast]") {
  const auto board = CreateBoard<2, 2>();

  std::vector<Move> moves{
      Move{1, 1, 1, 2},
      Move{1, 1, 2, 1},
  };

  for (const auto& move : moves) {
    REQUIRE_FALSE(MoveIsAllowedOn<2, 2>(board, move, Color::Blue));
  }
}

TEST_CASE("Moving a tower is illegal if a player does not own it", "[fast]") {
  const Board<2, 2> board;

  SECTION("First player moves second player's tower") {
    const Move move{0, 1, 0, 0};
    REQUIRE_FALSE(MoveIsAllowedOn<2, 2>(board, move, Color::Blue));
  }

  SECTION("Second player moves first player's tower") {
    const Move move{0, 0, 0, 1};
    REQUIRE_FALSE(MoveIsAllowedOn<2, 2>(board, move, Color::Yellow));
  }
}

TEST_CASE("Moving a tower is illegal if source and target are equal",
          "[fast]") {
  const Board<3, 4> board;  // arbitrary size

  const Position source{0, 0};
  const Position target{0, 0};
  const Move move{source, target};
  REQUIRE_FALSE(MoveIsAllowedOn(board, move, Color::Blue));
}

// 3. move not allowed if no target tower
// 4. move not allowed if no source tower

// In this scenario, there is only one tower on the board which belongs to
// the first player.
TEST_CASE("Newly created Board of size 1 should have value 1", "[fast]") {
  const Board<1, 1> board;
  REQUIRE(ComputeValueOf<1, 1>(board) == 1);
}

TEST_CASE("Newly created Board should have value 0", "[fast]") {
  const Board<2, 2> board;
  REQUIRE(ComputeValueOf<2, 2>(board) == 0);
}
