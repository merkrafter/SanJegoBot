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
  const Tower tower(Player::FIRST);
  REQUIRE(tower.Height() == 1);
}

TEST_CASE("Tower should store 1st player as owner correctly", "[fast]") {
  const Tower tower(Player::FIRST);
  REQUIRE(tower.Owner() == Player::FIRST);
}

TEST_CASE("Tower should store 2nd player as owner correctly", "[fast]") {
  const Tower tower(Player::SECOND);
  REQUIRE(tower.Owner() == Player::SECOND);
}

TEST_CASE("Top-left tower of newly created gamefield is first player's",
          "[fast]") {
  GameField field(2, 2);
  const auto topleft_tower = field.TowerAt(0, 0);
  REQUIRE(topleft_tower.Owner() == Player::FIRST);
}

TEST_CASE("Tower at (0,1) of newly created gamefield is second player's",
          "[fast]") {
  GameField field(2, 2);
  const auto topleft_tower = field.TowerAt(0, 1);
  REQUIRE(topleft_tower.Owner() == Player::SECOND);
}

TEST_CASE("Tower at (1,0) of newly created gamefield is second player's",
          "[fast]") {
  GameField field(2, 2);
  const auto topleft_tower = field.TowerAt(1, 0);
  REQUIRE(topleft_tower.Owner() == Player::SECOND);
}

TEST_CASE("Tower at (1,1) of newly created gamefield is first player's",
          "[fast]") {
  GameField field(2, 2);
  const auto topleft_tower = field.TowerAt(1, 1);
  REQUIRE(topleft_tower.Owner() == Player::FIRST);
}
