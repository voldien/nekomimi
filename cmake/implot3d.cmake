INCLUDE(FetchContent)

IF(NOT TARGET implot3d)
    FetchContent_Declare(implot3d_source
        GIT_REPOSITORY https://github.com/pthom/implot3d.git
        GIT_TAG v0.4
    )

    FetchContent_MakeAvailable(implot3d_source)


    ADD_LIBRARY(implot3d STATIC
        ${implot3d_source_SOURCE_DIR}/implot3d.cpp
        ${implot3d_source_SOURCE_DIR}/implot3d_items.cpp
        ${implot3d_source_SOURCE_DIR}/implot3d_meshes.cpp
    )

    TARGET_COMPILE_DEFINITIONS(implot3d PRIVATE IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
    TARGET_INCLUDE_DIRECTORIES(implot3d PUBLIC
        ${implot3d_source_SOURCE_DIR}
        ${IMGUI_ROOT_DIRECTORY}
        $<BUILD_INTERFACE:${implot3d_source_SOURCE_DIR}>
    )
    #TARGET_LINK_LIBRARIES(implot3d PRIVATE nekomimi-window)
ENDIF()
