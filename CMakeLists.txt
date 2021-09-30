CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
PROJECT(mimi-imgui)
include(GNUInstallDirs)


# Ouput directory for both library and executables.
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)


SET(OpenGL_GL_PREFERENCE "GLVND")
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
    MESSAGE(STATUS "Glew found: ${GLEW_LIBRARIES}.")
ENDIF()

IF(OPENGL_FOUND)
    MESSAGE(STATUS "OpenGL found: ${OPENGL_LIBRARIES}.")
ENDIF()

# Find package that is not part of a Module
FIND_PACKAGE(PkgConfig QUIET)
IF(PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE} -- version ${PKG_CONFIG_VERSION_STRING}.")
ELSE()
	MESSAGE(WARNING "Some library dependices may not be able to be found automatically.")
ENDIF()

IF(PKG_CONFIG_FOUND)
	pkg_check_modules(SDL2 REQUIRED sdl2)
ELSE()
ENDIF()

IF(SDL2_FOUND)
	MESSAGE(STATUS "Found sdl2: ${SDL2_LIBRARIES} -- version ${SDL2_VERSION}.")
ENDIF()


FILE(GLOB SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/UIComponents/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/GraphicBackend/*.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/backends/imgui_impl_vulkan.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/backends/imgui_impl_sdl.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/imgui_widgets.cpp
	${CMAKE_SOURCE_DIR}/extern/imgui/imgui.cpp
)
FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include/UIComponents/*.h
)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/extern/fvkcore EXCLUDE_FROM_ALL)


ADD_LIBRARY(mimi-window ${SOURCE_FILES} ${HEADER_FILES} )
TARGET_INCLUDE_DIRECTORIES (mimi-window  PRIVATE
	${CMAKE_SOURCE_DIR}/extern/imgui
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mimi-imgui/include>
	$<INSTALL_INTERFACE:mimi-imgui/include>
)


TARGET_COMPILE_OPTIONS(mimi-window PRIVATE ${SDL2_CFLAGS_OTHER})

TARGET_LINK_LIBRARIES(mimi-window PRIVATE vkcommon-core imtui-ncurses ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
ADD_DEPENDENCIES(mimi-window vkcommon-core )
