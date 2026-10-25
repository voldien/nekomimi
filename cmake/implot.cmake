INCLUDE(FetchContent)

IF(NOT TARGET implot)

    FetchContent_Declare(implot_source
        GIT_REPOSITORY https://github.com/pthom/implot.git
        GIT_TAG 851cdbdd60d1dfb64c23e987969694de0f547a6b
    )
    FetchContent_MakeAvailable(implot_source)


    ADD_LIBRARY(implot STATIC
        ${implot_source_SOURCE_DIR}/implot.cpp
        ${implot_source_SOURCE_DIR}/implot_items.cpp
    )
    TARGET_COMPILE_DEFINITIONS(implot PRIVATE IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
    TARGET_INCLUDE_DIRECTORIES(implot PUBLIC
        ${implot_source_SOURCE_DIR}
        ${IMGUI_ROOT_DIRECTORY}
        $<BUILD_INTERFACE:${implot_source_SOURCE_DIR}>
    )
    #TARGET_LINK_LIBRARIES(implot PRIVATE nekomimi-window)
ENDIF()
