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
  REQUIRE(tower.Height() == 1);
}

TEST_CASE("Tower should store 1st player as owner correctly", "[fast]") {
  const Tower tower(Color::Blue);
  REQUIRE(tower.Top() == Color::Blue);
}

TEST_CASE("Tower should store 2nd player as owner correctly", "[fast]") {
  const Tower tower(Color::Yellow);
  REQUIRE(tower.Top() == Color::Yellow);
}

TEST_CASE("Top-left tower of newly created board is first player's", "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({0, 0});
  REQUIRE(tower.value().Top() == Color::Blue);
}

TEST_CASE("Tower at (0,1) of newly created board is second player's",
          "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({0, 1});
  REQUIRE(tower.value().Top() == Color::Yellow);
}

TEST_CASE("Tower at (1,0) of newly created board is second player's",
          "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({1, 0});
  REQUIRE(tower.value().Top() == Color::Yellow);
}

TEST_CASE("Tower at (1,1) of newly created board is first player's", "[fast]") {
  Board<2, 2> board;
  const std::optional<Tower> tower = board.GetTowerAt({1, 1});
  REQUIRE(tower.value().Top() == Color::Blue);
}

TEST_CASE("Attaching towers adds their heights", "[fast]") {
  Tower source(Color::Blue);
  Tower target(Color::Yellow);
  target.Attach(source);
  REQUIRE(target.Height() == 2);
}

TEST_CASE("Attached towers keep their top brick", "[fast]") {
  SECTION("Yellow on top") {
    Tower source(Color::Yellow);
    Tower target(Color::Blue);
    target.Attach(source);
    REQUIRE(target.Top() == source.Top());
  }
  SECTION("Blue on top") {
    Tower source(Color::Blue);
    Tower target(Color::Yellow);
    target.Attach(source);
    REQUIRE(target.Top() == source.Top());
  }
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