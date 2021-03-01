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
#include "libsanjego/bot.hpp"
#include "libsanjego/gameobjects.hpp"

// To make the test cases more readable
using namespace libsanjego;

TEST_CASE("Should return the only possible move") {
  SECTION("As first player") {
    const Board<1, 2> board;
    const Color active_player = Color::Blue;
    const Move expected_move{{0, 0}, {0, 1}};

    FullExplorer<1, 2> explorer;
    const auto computed_move = explorer.Explore(board, active_player).best_move;

    REQUIRE(computed_move == expected_move);
  }

  SECTION("As second player") {
    const Board<1, 2> board;
    const Color active_player = Color::Yellow;
    const Move expected_move{{0, 1}, {0, 0}};

    FullExplorer<1, 2> explorer;
    const auto computed_move = explorer.Explore(board, active_player).best_move;

    REQUIRE(computed_move == expected_move);
  }
}

TEST_CASE("Should return skip when there is no legal move left", "[test]") {
  const Board<1, 1> board;
  FullExplorer<1, 1> explorer;
  SECTION("As first player") {
    const Color active_player = Color::Blue;
    const auto computed_move = explorer.Explore(board, active_player).best_move;

    REQUIRE(computed_move.IsSkip());
  }
  SECTION("As second player") {
    const Color active_player = Color::Yellow;
    const auto computed_move = explorer.Explore(board, active_player).best_move;

    REQUIRE(computed_move.IsSkip());
  }
}

TEST_CASE("Time spent should be non-negative", "[fast]") {
  const Board<5, 5> board;
  FullExplorer<5, 5> explorer;
  const auto active_player = Color::Blue;
  const auto time_spent = explorer.Explore(board, active_player).seconds_spent;
  REQUIRE(time_spent >= 0);
}