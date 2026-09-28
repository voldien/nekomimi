INCLUDE(FetchContent)

IF(NOT TARGET implot)

    FetchContent_Declare(implot_source
        GIT_REPOSITORY https://github.com/pthom/implot.git
        GIT_TAG v1.0
    )

    FetchContent_GetProperties(implot_source)

    IF(NOT implot_source_POPULATED)
        FetchContent_Populate(implot_source)

        ADD_LIBRARY(implot INTERFACE
            ${implot_source_SOURCE_DIR}/implot.cpp
            ${implot_source_SOURCE_DIR}/implot_items.cpp
        )
        TARGET_COMPILE_DEFINITIONS(implot INTERFACE IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
        TARGET_INCLUDE_DIRECTORIES(implot INTERFACE
            ${implot_source_SOURCE_DIR}
            $<BUILD_INTERFACE:${implot_source_SOURCE_DIR}>
        )
    ELSE()
        MESSAGE(WARNING "Could not find implot source code")
    ENDIF()

ENDIF()
