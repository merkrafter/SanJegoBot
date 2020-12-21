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
