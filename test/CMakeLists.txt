# Unit test cases for game objects
add_executable(test_gameobjects catch_main.cpp test_gameobjects.cpp)
target_link_libraries(test_gameobjects PRIVATE sanjego)
target_link_libraries(test_gameobjects PRIVATE Catch2::Catch2)
add_test(NAME TEST_GAMEOBJECTS COMMAND test_gameobjects)
