FILE(GLOB TEST_SOURCES "*.cpp")
FILE(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/source/game/*.cpp")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable(run_tests ${TEST_SOURCES} ${GAME_SOURCES})

include_directories("${CMAKE_SOURCE_DIR}/source/game")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

target_link_libraries(run_tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(UnitTests run_tests)
