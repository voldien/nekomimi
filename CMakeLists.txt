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


IF (NOT TARGET fragcore)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore EXCLUDE_FROM_ALL)
ENDIF()
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/observable/observable EXCLUDE_FROM_ALL)

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

TARGET_LINK_LIBRARIES(mimi-window PUBLIC observable fragcore fragcore-render-interface fragcore-shader-compiler fragcore-rgl fragcore-rvk ${GLEW_LIBRARIES} OpenGL::OpenGL OpenGL::GLU OpenGL::EGL  )
ADD_DEPENDENCIES(mimi-window observable fragcore fragcore-render-interface fragcore-rgl fragcore-rvk)

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8) # -fPIC is only required
	SET_PROPERTY(TARGET mimi-window PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

########################
# Samples codes
########################
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/samples)

INSTALL(TARGETS mimi-window DESTINATION lib COMPONENT ${LIBMIMIIMGUIFRAGCORE_COMPONENT})

