# Source files for the gl bunny program.
FILE(GLOB MIMI_SAMPLE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# nekomimi Sample Executable
ADD_EXECUTABLE(nekomimi-example ${MIMI_SAMPLE_SOURCE_FILES})
TARGET_LINK_LIBRARIES(nekomimi-example PRIVATE nekomimi-window)
TARGET_INCLUDE_DIRECTORIES(nekomimi-example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
ADD_DEPENDENCIES(nekomimi-example nekomimi-window)
