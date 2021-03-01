#include <cstdint>
#include <vector>

#include "catch2/catch.hpp"
#include "libsanjego/gameobjects.hpp"
#include "libsanjego/rulesets.hpp"

// To make the test cases more readable
using namespace libsanjego;

TEST_CASE("Basic move is allowed", "[fast]") {
  const Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  const Move move = {0, 0, 0, 1};
  REQUIRE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
}

TEST_CASE("On a 1x1 board, any move is illegal", "[fast]") {
  const Board<1, 1> board;
  auto ruleset = CreateStandardRulesetFor(board);

  Move move{0, 0, 0, 1};
  REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
}

TEST_CASE("Moves over border are illegal", "[fast]") {
  const auto board = CreateBoard<2, 2>();
  auto ruleset = CreateStandardRulesetFor(board);

  std::vector<Move> moves{
      Move{1, 1, 1, 2},
      Move{1, 1, 2, 1},
  };

  for (const auto &move : moves) {
    REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
  }
}

TEST_CASE("Moving a tower is illegal if a player does not own it", "[fast]") {
  const Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  SECTION("First player moves second player's tower") {
    const Move move{0, 1, 0, 0};
    REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
  }

  SECTION("Second player moves first player's tower") {
    const Move move{0, 0, 0, 1};
    REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Yellow));
  }
}

TEST_CASE("Moving a tower is illegal if source and target are equal",
          "[fast]") {
  const Board<3, 4> board;  // arbitrary size
  auto ruleset = CreateStandardRulesetFor(board);

  const Position source{0, 0};
  const Position target{0, 0};
  const Move move{source, target};
  REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
}

// 3. move not allowed if no target tower
// 4. move not allowed if no source tower

// In this scenario, there is only one tower on the board which belongs to
// the first player.
TEST_CASE("Newly created board of size 1 should have value 1", "[fast]") {
  const Board<1, 1> board;
  auto ruleset = CreateStandardRulesetFor(board);
  REQUIRE(ruleset->ComputeValueOf(board) == 1);
}

TEST_CASE("Newly created board should have value 0", "[fast]") {
  const Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);
  REQUIRE(ruleset->ComputeValueOf(board) == 0);
}

TEST_CASE("After first move, the board should have a value of 1", "[fast]") {
  Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  Move arbitrary_legal_move{{0, 0}, {0, 1}};
  board.Make(arbitrary_legal_move);

  // REQUIRE(ruleset->ComputeValueOf(board) == 1);
}

TEST_CASE("GetLegalMoves should return empty vector in an end state",
          "[fast]") {
  const Board<1, 1> board;
  auto ruleset = CreateStandardRulesetFor(board);

  SECTION("First player") {
    const auto legalMoves = ruleset->GetLegalMoves(board, Color::Blue);

    REQUIRE(legalMoves.empty());
  }

  SECTION("Second player") {
    const auto legalMoves = ruleset->GetLegalMoves(board, Color::Yellow);

    REQUIRE(legalMoves.empty());
  }
}

TEST_CASE("On a 2x2 board, each player has 4 possible opening moves",
          "[fast]") {
  const Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  SECTION("First player") {
    const auto legalMoves = ruleset->GetLegalMoves(board, Color::Blue);

    REQUIRE(legalMoves.size() == 4);
  }

  SECTION("Second player") {
    const auto legalMoves = ruleset->GetLegalMoves(board, Color::Yellow);

    REQUIRE(legalMoves.size() == 4);
  }
}