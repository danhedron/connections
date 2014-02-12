## Qt 5 CMake information via http://www.kdab.com/using-cmake-with-qt-5/

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

FILE(GLOB GAME_SOURCES "game/*.cpp")
FILE(GLOB APP_SOURCES "app/*.cpp")
FILE(GLOB APP_HEADERS "app/*.hpp")

add_executable(connections ${GAME_SOURCES} ${APP_SOURCES})
qt5_use_modules(connections Widgets)