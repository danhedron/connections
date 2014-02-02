
# Find Qt
find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )
ADD_DEFINITIONS(${QT_DEFINITIONS})

FILE(GLOB GAME_SOURCES "game/*.cpp")
FILE(GLOB APP_SOURCES "app/*.cpp")
FILE(GLOB APP_HEADERS "app/*.hpp")
QT4_WRAP_CPP(APP_HEADERS_PROCESSED ${APP_HEADERS})
add_executable(connections ${GAME_SOURCES} ${APP_SOURCES} ${APP_HEADERS_PROCESSED})

target_link_libraries(connections ${QT_LIBRARIES})
include_directories(${QT_INCLUDE_DIR})
