/*
 * Copyright 2021 merkrafter
 *
 * This file is part of libsanjego.
 *
 * libsanjego is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsanjego is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libsanjego. If not, see <https://www.gnu.org/licenses/>.
 */
#include <cstdint>

#include "catch2/catch.hpp"
#include "libsanjego/gameobjects.hpp"

// To make the test cases more readable
using namespace libsanjego;

TEST_CASE("Newly created tower should have height 1", "[fast]") {
  const Tower tower(Color::Blue);
  REQUIRE(tower.height() == 1);
}

TEST_CASE("Tower should store 1st player as owner correctly", "[fast]") {
  const Tower tower(Color::Blue);
  REQUIRE(tower.top() == Color::Blue);
}

TEST_CASE("Tower should store 2nd player as owner correctly", "[fast]") {
  const Tower tower(Color::Yellow);
  REQUIRE(tower.top() == Color::Yellow);
}

TEST_CASE("top-left tower of newly created board is first player's", "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({0, 0});
  REQUIRE(tower.value().top() == Color::Blue);
}

TEST_CASE("Tower at (0,1) of newly created board is second player's",
          "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({0, 1});
  REQUIRE(tower.value().top() == Color::Yellow);
}

TEST_CASE("Tower at (1,0) of newly created board is second player's",
          "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({1, 0});
  REQUIRE(tower.value().top() == Color::Yellow);
}

TEST_CASE("Tower at (1,1) of newly created board is first player's", "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({1, 1});
  REQUIRE(tower.value().top() == Color::Blue);
}

TEST_CASE("Attaching towers adds their heights", "[fast]") {
  Tower source(Color::Blue);
  Tower target(Color::Yellow);
  target.Attach(source);
  REQUIRE(target.height() == 2);
}

TEST_CASE("Attached towers keep their top brick", "[fast]") {
  SECTION("Yellow on top") {
    Tower source(Color::Yellow);
    Tower target(Color::Blue);
    target.Attach(source);
    REQUIRE(target.top() == source.top());
  }
  SECTION("Blue on top") {
    Tower source(Color::Blue);
    Tower target(Color::Yellow);
    target.Attach(source);
    REQUIRE(target.top() == source.top());
  }
}

TEST_CASE("Detaching towers subtracts their heights", "[fast]") {
  const Tower source(Color::Blue);
  Tower target(Color::Yellow);
  const Tower original_target(target);

  target.Attach(source);

  target.DetachFrom(original_target);
  REQUIRE(target.height() == source.height());
}

TEST_CASE("Detaching towers restores top brick", "[fast]") {
  const Tower source(Color::Blue);
  Tower target(Color::Yellow);
  const Tower original_target(target);

  target.Attach(source);

  target.DetachFrom(original_target);
  REQUIRE(target.top() == source.top());
}

TEST_CASE("Positions are equal if both coordinates are equal", "[fast]") {
  const Position pos1{0, 1};
  const Position pos2{0, 1};
  REQUIRE(pos1 == pos2);
}

TEST_CASE("Positions are not equal if their coordinates are swapped",
          "[fast]") {
  const Position pos1{0, 1};
  const Position pos2{1, 0};
  REQUIRE_FALSE(pos1 == pos2);
}

TEST_CASE("Skipping is a valid move", "[fast]") {
  const auto move = Move::Skip();
  REQUIRE(move.IsSkip());
}

TEST_CASE("Normal moves don't signal skipping", "[fast]") {
  std::vector<Move> moves{
      Move{0, 0, 0, 1},
      Move{1, 0, 0, 0},
      Move{1, 1, 1, 2},
      Move{2, 3, 3, 4},
  };
  for (auto &move : moves) {
    REQUIRE_FALSE(move.IsSkip());
  }
}

TEST_CASE("Moves are equal if source and target positions are equal",
          "[fast]") {
  const Move move1{{0, 1}, {1, 0}};
  const Move move2{{0, 1}, {1, 0}};
  REQUIRE(move1 == move2);
}

TEST_CASE("Moves are not equal if sources differ", "[fast]") {
  SECTION("columns differ") {
    const Move move1{{0, 1}, {1, 0}};
    const Move move2{{0, 2}, {1, 0}};
    REQUIRE_FALSE(move1 == move2);
  }
  SECTION("rows differ") {
    const Move move1{{0, 1}, {1, 0}};
    const Move move2{{1, 1}, {1, 0}};
    REQUIRE_FALSE(move1 == move2);
  }
}

TEST_CASE("Moves are not equal if targets differ", "[fast]") {
  SECTION("columns differ") {
    const Move move1{{0, 1}, {1, 0}};
    const Move move2{{0, 1}, {1, 3}};
    REQUIRE_FALSE(move1 == move2);
  }
  SECTION("rows differ") {
    const Move move1{{0, 1}, {0, 0}};
    const Move move2{{0, 1}, {1, 0}};
    REQUIRE_FALSE(move1 == move2);
  }
}

TEST_CASE("Moving towers adds the heights of source and target", "[fast]") {
  Board<2, 2> board;  // initializes board with height 1 towers
  const Position source{0, 0};
  const Position target{0, 1};
  Move move{source, target};
  board.Make(move);
  const std::optional<Tower> resulting_tower = board.GetTowerAt(target);
  REQUIRE(resulting_tower.value().height() == 2);
}

TEST_CASE("Moved towers keep their top brick's color", "[fast]") {
  Board<2, 2> board;  // initializes board with blue tower at (0,0)
  const Position source{0, 0};
  const Position target{0, 1};
  const auto source_tower_color = board.GetTowerAt(source).value().top();

  Move move{source, target};
  board.Make(move);
  const std::optional<Tower> resulting_tower = board.GetTowerAt(target);
  REQUIRE(resulting_tower.value().top() == source_tower_color);
}

TEST_CASE("'Moving' towers from and to the same position fails", "[fast]") {
  Board<2, 2> board;  // initializes board with blue tower at (0,0)
  const Position source{0, 0};
  const Position target{0, 0};
  Move move{source, target};
  const auto success = board.Make(move);
  REQUIRE_FALSE(success);
}

TEST_CASE("Moving towers outside the boundaries fails", "[fast]") {
  Board<2, 2> board;
  SECTION("source with wrong row") {
    const Position source{-1, 0};
    const Position target{0, 1};
    Move move{source, target};
    const auto success = board.Make(move);
    REQUIRE_FALSE(success);
  }
  SECTION("source with wrong column") {
    const Position source{0, 3};
    const Position target{0, 1};
    Move move{source, target};
    const auto success = board.Make(move);
    REQUIRE_FALSE(success);
  }
  SECTION("target with wrong row") {
    const Position source{0, 1};
    const Position target{4, 0};
    Move move{source, target};
    const auto success = board.Make(move);
    REQUIRE_FALSE(success);
  }
  SECTION("target with wrong column") {
    const Position source{0, 1};
    const Position target{0, -3};
    Move move{source, target};
    const auto success = board.Make(move);
    REQUIRE_FALSE(success);
  }
}

TEST_CASE("After being moved, the source tower's position is empty", "[fast]") {
  Board<2, 2> board;
  const Position source{0, 0};
  const Position target{0, 1};

  Move move{source, target};
  board.Make(move);

  const std::optional<Tower> source_field = board.GetTowerAt(source);
  REQUIRE_FALSE(source_field.has_value());
}

TEST_CASE("Moves can be reverted", "[fast]") {
  Board<2, 2> board;
  const Position source{0, 0};
  const Position target{0, 1};

  const auto original_source_tower{board.GetTowerAt(source)};
  const auto original_target_tower{board.GetTowerAt(target)};

  Move move{source, target};
  board.Make(move);
  const auto success = board.Undo(move);
  REQUIRE(success);

  const auto source_tower = board.GetTowerAt(source);
  const auto target_tower = board.GetTowerAt(target);

  // original configuration should have been restored
  REQUIRE(source_tower.has_value());
  REQUIRE(source_tower->height() == original_source_tower->height());
  REQUIRE(source_tower->top() == original_source_tower->top());

  REQUIRE(target_tower.has_value());
  REQUIRE(target_tower->height() == original_target_tower->height());
  REQUIRE(target_tower->top() == original_target_tower->top());
}

TEST_CASE("Move reversal fails outside the board's borders", "[fast]") {
  Board<2, 2> board;
  const Position source{0, 0};
  const Position target{0, 1};
  const Position invalid{-1, -1};

  Move move{source, target};
  REQUIRE(board.Make(move));

  move.source = invalid;

  const auto success = board.Undo(move);
  REQUIRE_FALSE(success);
}

TEST_CASE("Move reversal fails if the source is occupied", "[fast]") {
  Board<2, 2> board;
  const Position source{0, 0};
  const Position target{0, 1};
  const Position unrelated{1, 1};

  Move move{source, target};
  REQUIRE(board.Make(move));

  move.source = unrelated;

  const auto success = board.Undo(move);
  REQUIRE_FALSE(success);
}
