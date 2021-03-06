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

TEST_CASE("Moving a tower is illegal if source is empty", "[fast]") {
  Board<3, 4> board;  // arbitrary size
  auto ruleset = CreateStandardRulesetFor(board);

  const Position source{2, 2};
  const Position target{2, 1};
  Move move{source, target};
  board.Make(move);

  // now source is empty

  REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, move, Color::Blue));
}

TEST_CASE("Moving a tower is illegal if target is empty", "[fast]") {
  Board<3, 4> board;  // arbitrary size
  auto ruleset = CreateStandardRulesetFor(board);

  const Position source{2, 2};
  const Position target{2, 1};
  Move move{source, target};
  board.Make(move);

  // now source is empty

  const Move counter{{2, 3}, source};

  REQUIRE_FALSE(ruleset->MoveIsAllowedOn(board, counter, Color::Yellow));
}

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

TEST_CASE("Better position for blue have a positive game value", "[fast]") {
  Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  // moves a blue tower
  Move arbitrary_legal_move{{0, 0}, {0, 1}};
  board.Make(arbitrary_legal_move);

  REQUIRE(ruleset->ComputeValueOf(board) > 0);
}

TEST_CASE("Better position for yellow have a negative game value", "[fast]") {
  Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  // moves a yellow tower
  Move arbitrary_legal_move{{0, 1}, {0, 0}};
  board.Make(arbitrary_legal_move);

  REQUIRE(ruleset->ComputeValueOf(board) < 0);
}

TEST_CASE("After first move, the board should have a value of 1", "[fast]") {
  Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  Move arbitrary_legal_move{{0, 0}, {0, 1}};
  board.Make(arbitrary_legal_move);

  REQUIRE(ruleset->ComputeValueOf(board) == 1);
}

TEST_CASE("GetLegalMoves should return empty vector in an end state",
          "[fast]") {
  const Board<1, 1> board;
  auto ruleset = CreateStandardRulesetFor(board);

  SECTION("First player") {
    const auto legal_moves = ruleset->GetLegalMoves(board, Color::Blue);

    REQUIRE(legal_moves.empty());
  }

  SECTION("Second player") {
    const auto legal_moves = ruleset->GetLegalMoves(board, Color::Yellow);

    REQUIRE(legal_moves.empty());
  }
}

TEST_CASE("On a 2x2 board, each player has 4 possible opening moves",
          "[fast]") {
  const Board<2, 2> board;
  auto ruleset = CreateStandardRulesetFor(board);

  SECTION("First player") {
    const auto legal_moves = ruleset->GetLegalMoves(board, Color::Blue);

    REQUIRE(legal_moves.size() == 4);
  }

  SECTION("Second player") {
    const auto legal_moves = ruleset->GetLegalMoves(board, Color::Yellow);

    REQUIRE(legal_moves.size() == 4);
  }
}

TEST_CASE("A player owns a tower iff the top brick has their color", "[fast]") {
  StandardRuleset<2, 3> ruleset;  // arbitrary size
  SECTION("blue") {
    const Tower blue_tower{Color::Blue};
    REQUIRE(ruleset.OwnsTower(Color::Blue, blue_tower));
    REQUIRE_FALSE(ruleset.OwnsTower(Color::Yellow, blue_tower));
  }
  SECTION("yellow") {
    const Tower yellow_tower{Color::Yellow};
    REQUIRE(ruleset.OwnsTower(Color::Yellow, yellow_tower));
    REQUIRE_FALSE(ruleset.OwnsTower(Color::Blue, yellow_tower));
  }
}