CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
PROJECT(mimi-imgui)
INCLUDE(GNUInstallDirs)

# Ouput directory for both library and executables.
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)


OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ON)
IF(NOT BUILD_SHARED_LIBS)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
	MESSAGE(STATUS "Shared libraries enabled")
ENDIF(NOT BUILD_SHARED_LIBS)



################################################
# Find all dependices libraries and etc.
################################################
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

# Find SDL and GLFW3
IF(PKG_CONFIG_FOUND)
	pkg_check_modules(SDL2 sdl2)
	pkg_check_modules(GLFW glfw3)
ELSE()
ENDIF()

IF(SDL2_FOUND)
	MESSAGE(STATUS "Found sdl2: ${SDL2_LIBRARIES} -- version ${SDL2_VERSION}.")
ENDIF()

IF(GLFW_FOUND)
	MESSAGE(STATUS "Found glfw: ${GLFW_LIBRARIES} -- version ${GLFW_VERSION}.")
ENDIF()

IF (NOT TARGET fragcore)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore EXCLUDE_FROM_ALL)
ENDIF()

FILE(GLOB SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/UIComponents/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/util/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/GraphicBackend/WindowBackend*.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_vulkan.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_sdl.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_dx9.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui.cpp
)

FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include/UIComponents/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include/GraphicBackend/*.h
)

#######################################
# SDL2 Window Backend Impl
######################################
ADD_LIBRARY(mimi-sdl2 OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/GraphicBackend/SDLWindow.cpp)
TARGET_INCLUDE_DIRECTORIES(mimi-sdl2 PUBLIC ${SDL2_INCLUDE_DIRS} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include
)
TARGET_COMPILE_OPTIONS(mimi-sdl2 PRIVATE  "-fPIC")
TARGET_LINK_LIBRARIES(mimi-sdl2 PUBLIC  ${SDL2_LIBRARIES} fragcore)
TARGET_COMPILE_OPTIONS(mimi-sdl2 PRIVATE ${SDL2_CFLAGS_OTHER} )


#######################################
# GLFW3 Window Backend Impl
######################################
ADD_LIBRARY(mimi-glfw OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/src/GraphicBackend/glwf3Window.cpp)
TARGET_INCLUDE_DIRECTORIES(mimi-glfw PUBLIC ${GLFW_INCLUDE_DIRS} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include)
TARGET_COMPILE_OPTIONS(mimi-glfw PRIVATE  "-fPIC")
TARGET_LINK_LIBRARIES(mimi-glfw PUBLIC  ${GLFW_LIBRARIES} fragcore)
TARGET_COMPILE_OPTIONS(mimi-glfw PRIVATE ${GLFW_CFLAGS_OTHER} )

#########################################
# Core Window Management Library.
#########################################
ADD_LIBRARY(mimi-window ${SOURCE_FILES} ${HEADER_FILES} )
TARGET_COMPILE_OPTIONS(mimi-window  PRIVATE  "-fPIC")
TARGET_INCLUDE_DIRECTORIES (mimi-window  PRIVATE
	${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/extern
	${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mimi-imgui/include>
	$<INSTALL_INTERFACE:mimi-imgui/include>
)


TARGET_LINK_LIBRARIES(mimi-window PUBLIC mimi-sdl2 fragcore fragcore-render-interface fragcore-rgl fragcore-rvk )
ADD_DEPENDENCIES(mimi-window vkcommon-core fragcore fragcore-render-interface fragcore-rgl fragcore-rvk)


#####
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/samples)
