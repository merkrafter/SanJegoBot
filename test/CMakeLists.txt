# Unit test cases for game objects
add_executable(test_gameobjects catch_main.cpp test_gameobjects.cpp)
target_link_libraries(test_gameobjects PRIVATE sanjego)
target_link_libraries(test_gameobjects PRIVATE Catch2::Catch2)
add_test(NAME TEST_GAMEOBJECTS COMMAND test_gameobjects)

# Unit test cases for rulesets
add_executable(test_rulesets catch_main.cpp test_rulesets.cpp)
target_link_libraries(test_rulesets PRIVATE sanjego)
target_link_libraries(test_rulesets PRIVATE Catch2::Catch2)
add_test(NAME TEST_RULESETS COMMAND test_rulesets)

# Unit test cases for bots
add_executable(test_bots catch_main.cpp test_bot.cpp)
target_link_libraries(test_bots PRIVATE sanjego)
target_link_libraries(test_bots PRIVATE Catch2::Catch2)
add_test(NAME TEST_BOTS COMMAND test_bots)