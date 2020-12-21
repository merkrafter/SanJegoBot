/*
 * This file follows the suggestion from
 * https://github.com/catchorg/Catch2/blob/devel/docs/slow-compiles.md
 *
 * As Catch is a header-only library.
 * Therefore, each (test) file it is included into needs to compile the whole
 * Catch library every time some test code changes, which takes a long time.
 * Having the #include in this dedicated test_main file avoids the repeated
 * recompilation of the Catch header.
 */
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
