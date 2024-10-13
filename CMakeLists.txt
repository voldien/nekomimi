CMAKE_MINIMUM_REQUIRED( VERSION 3.9...3.20)
PROJECT(nekomimi CXX C)
INCLUDE(GNUInstallDirs)

# Ouput directory for both library and executables.
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

## Shared Library.
OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ON)
IF(NOT BUILD_SHARED_LIBS)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
	MESSAGE(STATUS "Shared libraries enabled")
ENDIF(NOT BUILD_SHARED_LIBS)

########################################
#	Build Options
#########################################
OPTION(MIMI_OPENGL_SUPPORT "Enable OpenGL Backend" ON)
OPTION(MIMI_VULKAN_SUPPORT "Enable Vulkan Backend" ON)
OPTION(MIMI_TERMINAL_SUPPORT "Enable Terminal Backend" ON)
OPTION(MIMI_DIRECTX_SUPPORT "Enable DirectX Backend" OFF)

OPTION(MIMI_WINDOW_SDL2_SUPPORT "Enable SDL2 Backend" OFF)
OPTION(MIMI_WINDOW_GLFW3_SUPPORT "Enable GLFW3 Backend" OFF)

################################################
# Find all dependices libraries and etc.
################################################
IF(MIMI_OPENGL_SUPPORT)
	SET(OpenGL_GL_PREFERENCE "GLVND")
	FIND_PACKAGE(OpenGL REQUIRED)

	IF(OPENGL_FOUND)
		MESSAGE(STATUS "OpenGL found: ${OPENGL_LIBRARIES}.")
	ENDIF()
ENDIF()

# Find package that is not part of a Module
FIND_PACKAGE(PkgConfig QUIET)
IF(PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE} -- version ${PKG_CONFIG_VERSION_STRING}.")
ELSE()
	MESSAGE(WARNING "Some library dependices may not be able to be found automatically.")
ENDIF()

############################################
# Backend libraries
############################################

IF (NOT TARGET fragcore)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore EXCLUDE_FROM_ALL)
ENDIF()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/window EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/window/sdl EXCLUDE_FROM_ALL)	#
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/window/glfw3 EXCLUDE_FROM_ALL)	#
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/image EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/renderer EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/renderer/opengl EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/fragcore/modules/renderer/vulkan EXCLUDE_FROM_ALL)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/observable/observable EXCLUDE_FROM_ALL)

###################################
# Source Code For Main Libary.
###################################
FILE(GLOB SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/src/UIComponents/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/src/util/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/src/GraphicBackend/WindowBackend*.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_sdl2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/misc/cpp/*.cpp
)

IF(MIMI_OPENGL_SUPPORT)
	FILE(GLOB BACKEND_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp) 
ENDIF()

IF(MIMI_VULKAN_SUPPORT)
	FILE(GLOB BACKEND_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_vulkan.cpp)
ENDIF()

#Only add directX for Windows platforms.
IF(WINDOWS AND MIMI_DIRECTX_SUPPORT)
	FILE(GLOB BACKEND_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_dx9.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_dx10.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_dx11.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_dx12.cpp
	)
ENDIF()

IF(ANDROID)
	FILE(GLOB APPEND BACKEND_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_android.cpp
	)
ENDIF()

# Headers
FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/include/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/include/UIComponents/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/include/GraphicBackend/*.h
)

#########################################
# Core Window Management Library.
#########################################
ADD_LIBRARY(nekomimi-window ${SOURCE_FILES} ${BACKEND_SOURCE_FILES} ${HEADER_FILES} )
TARGET_INCLUDE_DIRECTORIES (nekomimi-window  PRIVATE
	${OPENGL_INCLUDE_DIR}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/extern
	${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/include
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/nekomimi/include>
	$<INSTALL_INTERFACE:nekomimi/include>
)

TARGET_LINK_LIBRARIES(nekomimi-window PUBLIC observable fragcore-render-interface fragcore-shader-compiler)
ADD_DEPENDENCIES(nekomimi-window observable fragcore-render-interface)

IF(MIMI_OPENGL_SUPPORT)
	TARGET_COMPILE_DEFINITIONS(nekomimi-window PRIVATE MIMI_IMPL_OPENGL)
	TARGET_LINK_LIBRARIES(nekomimi-window PUBLIC fragcore-rgl glew OpenGL::OpenGL OpenGL::GLU OpenGL::EGL)
ENDIF()

IF(MIMI_VULKAN_SUPPORT)
	TARGET_COMPILE_DEFINITIONS(nekomimi-window PRIVATE MIMI_IMPL_VULKAN)
	TARGET_LINK_LIBRARIES(nekomimi-window PUBLIC fragcore-rvk)
ENDIF()

IF(MIMI_DIRECTX_SUPPORT)
	TARGET_COMPILE_DEFINITIONS(nekomimi-window PRIVATE MIMI_IMPL_DX9)
ENDIF()

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8) # -fPIC is only required
	SET_PROPERTY(TARGET nekomimi-window PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

########################
# Samples codes
########################
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/samples)

INSTALL(TARGETS nekomimi-window DESTINATION lib)