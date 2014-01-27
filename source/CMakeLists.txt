FILE(GLOB GAME_SOURCES "game/*.cpp")
FILE(GLOB APP_SOURCES "app/*.cpp")
add_executable(connections ${GAME_SOURCES} ${APP_SOURCES})

