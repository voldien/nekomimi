# Source files for the gl bunny program.
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


# Executable OpenGL hpm benchmark.
ADD_EXECUTABLE(mimi-example ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(mimi-example PRIVATE mimi-window vkcommon-core)
TARGET_INCLUDE_DIRECTORIES(mimi-example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
ADD_DEPENDENCIES(mimi-example mimi-window)

