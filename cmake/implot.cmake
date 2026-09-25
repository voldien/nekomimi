INCLUDE(FetchContent)

IF(NOT TARGET implot_source)

	FetchContent_Declare(implot_source
		GIT_REPOSITORY https://github.com/pthom/implot.git
		GIT_TAG "bundle_20260813"
	)
	
	FetchContent_GetProperties(implot_source)

	IF(NOT implot_source_POPULATED)
		FetchContent_Populate(implot_source)

        #general settings
        FILE(GLOB IMGUI_SOURCES
            ${implot_source_SOURCE_DIR}/implot.cpp
            ${implot_source_SOURCE_DIR}/implot_items.cpp        )

        ADD_LIBRARY(implot INTERFACE ${IMGUI_SOURCES})

        TARGET_COMPILE_DEFINITIONS(implot INTERFACE IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
        TARGET_INCLUDE_DIRECTORIES(implot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


	ELSE()
		MESSAGE(WARNING "Could not find implot source code")
	ENDIF()
ENDIF()



